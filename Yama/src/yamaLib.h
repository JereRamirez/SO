#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#define RQ_ARCH_FS 4567
#define RQ_BLCK_FS 3446
#define RQ_WRKS_FS 3452

typedef struct {
    int32_t* socketMaster;
    pthread_t* hilo;
    bool state;
}MasterPTH;

typedef struct {
    int32_t job;
    int32_t master;
    char*	nodo;
    int32_t bloque;
    int32_t estado;
    int32_t etapa;
    char* archTemp;
}tablaEst;

typedef struct {
    int32_t id;
    int32_t next_id;
    char*   nombre;
    int32_t puertoWorker;
    char*   ipWorker;
    int32_t estado;
    int32_t disponibilidad;
    int32_t clock;
}NodoFs;


typedef struct{ //FILESYSTEM{
        int32_t bloque_cp0;
        char*   nodo_cp0;
        int32_t bloque_cp1;
        char*   nodo_cp1;
        int32_t tamanio;
        int32_t copyInUse;
}BlockFs;

bool listaContineStr(t_list* lista,char* valor){
    int i;
    tablaEst* asign;
    for(i=0;i<list_size(lista);i++){
        asign = list_get(lista,i);
        if(!strcmp(asign->nodo,valor))
            return true;
    }
    return false;
}


int32_t cargaNodo(NodoFs* node,t_list* tabla){
    int i,carga=0;
    for(i=0;i<list_size(tabla);i++){
        tablaEst* aux=list_get(tabla,i);
        if(!strcmp(aux->nodo,node->nombre))
            carga++;
    }
    return carga;
}

int32_t wlMax(t_list* tabla){
    t_list* nodosTabla = list_create();
    int i;
    char *asign;
    for(i=0;i<list_size(tabla);i++){
        tablaEst* aux=list_get(tabla,i);
        if(!listaContineStr(nodosTabla,aux->nodo)){
            asign=malloc(sizeof(strlen(aux->nodo)));
            strcpy(asign,aux->nodo);
            list_add(nodosTabla,asign);
            asign = NULL;
        }
    }
    int32_t max=0;
    for(i=0;i<list_size(nodosTabla);i++){
        tablaEst* aux=list_get(tabla,i);

        if(cargaNodo(aux->nodo,tabla) > max){
            max=cargaNodo(aux->nodo,tabla);
        }
    }
    list_destroy(nodosTabla);
    return max;
}

int32_t pwl(NodoFs* nodo,t_list* listaEstados){
    return wlMax(listaEstados) - cargaNodo(nodo,listaEstados);
}

u_int32_t availability(NodoFs* nodo,int base,t_list* listaEstados,char* algoritm){

    if(!strcmp("WCLOCK",algoritm))
        return base + pwl(nodo,listaEstados);
    if(!strcmp("CLOCK",algoritm))
        return base;
    return 0;
}

void recibeString(int32_t socket, char* aux,t_log* logger){
    int32_t data;
    recibir(socket,&data,sizeof(int32_t),logger);
    aux = malloc(data+1);
    recibir(socket,aux,data,logger);
    aux[data]= '\0';
}

t_list* arrayNodos(int32_t socketFs,t_log *logger){
    int32_t bytes,i,size,msg=RQ_WRKS_FS;
    enviar(socketFs,&msg,sizeof(int32_t),logger);
    bytes = recibir(socketFs,&size,sizeof(int32_t),logger);
    if(bytes<=0)
        return NULL;
    t_list* nodos = list_create();
    NodoFs* aux;
    for(i=0;i<size;i++){
        aux= malloc(sizeof(NodoFs));
        recibeString(socketFs,(aux->nombre),logger);
        recibir(socketFs,&(aux->puertoWorker),sizeof(int32_t),logger);
        recibeString(socketFs,(aux->ipWorker),logger);
        aux->estado =1;
        aux->clock=0;
        aux->next_id=-1;
        aux->id=i;
        aux->disponibilidad =0;
        list_add(nodos,aux);
        aux= NULL;
    }
    return aux;
}

BlockFs* arrayBlockFs(int32_t socketFs,int size,t_log* logger){
    BlockFs *archivo = malloc(sizeof(BlockFs)*size);
    int i;
    for(i=0;i<size;i++){
        recibir(socketFs,&archivo[i].bloque_cp0,sizeof(int32_t),logger);
        recibeString(socketFs,archivo[i].nodo_cp0,logger);
        recibir(socketFs,&archivo[i].bloque_cp1,sizeof(int32_t),logger);
        recibeString(socketFs,archivo[i].nodo_cp1,logger);
        recibir(socketFs,&archivo[i].tamanio,sizeof(int32_t),logger);
        archivo[i].copyInUse=-1;
    }
    return archivo;
}

char* numToEtapa(int etapa){
    char *retorno;
    switch(etapa){
        case SENTINELA:
            retorno = "SENTINELA";
            break;
        case EN_ESPERA:
            retorno = "EN ESPERA";
            break;
        case EN_PROCESO:
            retorno = "EN PROCESO";
            break;
        case FINALIZADO_OK:
            retorno = "FINALIZADO OK";
            break;
        case ERROR:
            retorno = "ERROR";
            break;
    }
    return retorno;
}

char* numToEstado(int estado){
    char *retorno;
    switch(estado){
        case TRANSFORMACION:
            retorno = "TRANSFORMACION";
            break;
        case REDUCCION_LOCAL:
            retorno = "REDUCCION LOCAL";
            break;
        case REDUCCION_GLOBAL:
            retorno = "REDUCCION GLOBAL";
            break;
        case ALMACENAMIENTO:
            retorno = "ALMACENAMIENTO FINAL";
            break;
    }
    return retorno;
}

void printTablaEstador(t_list* tabla){
    int i;
    tablaEst *aux;
    printf("|Job\t|Master\t|Nodo\t|Bloque\t|Etapa\t|Archivo Temporal\t\t|Estado|");
    for(i=0;i<list_size(tabla);i++){
        aux = list_get(tabla,i);
        printf("|%d\t|%d\t|%d\t|%d\t|%s\t|%s\t\t|%s|",aux->job,aux->master,aux->nodo,aux->bloque,numToEtapa(aux->etapa),aux->archTemp,numToEstado(aux->estado));
    }
    printf("\n \n \n");
}



/*
 * planificador CLOCK
 *
 *
 *
 *
 */

bool prioridadAvailability(void* elem1, void* elem2){
    NodoFs* aux1 = (NodoFs*)elem1;
    NodoFs* aux2 = (NodoFs*)elem2;
    return aux1->disponibilidad>aux2->disponibilidad;
}

void availabilityNodos(int32_t base,char* alg,t_list* nodos, t_list* tablaEst){
    NodoFs* aux;
    int i;
    for(i=0;i<list_size(nodos);i++){
        aux=list_get(nodos,i);
        aux->disponibilidad = availability(aux,base,tablaEst,alg);
    }
    list_sort(nodos,prioridadAvailability);
    for(i=0;i<list_size(nodos);i++){
        aux = list_get(nodos,i);
        aux->id=i;
        if(i==0)
            aux->clock = 1;
        else
            aux->clock = 0;
        if(i == list_size(nodos) -1)
            aux->next_id = 0;
        else
            aux->next_id = i+1;
    }
}