#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libConexion.h"
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#define RQ_ARCH_FS 4567
#define RQ_BLCK_FS 3446

typedef struct{ //FILESYSTEM{
        int32_t bloque_cp0;
        int32_t nodo_cp0;
        int32_t bloque_cp1;
        int32_t nodo_cp1;
        int32_t tamanio;
}BlockFs;


BlockFs* arrayBlockFs(int size,t_log* logger){
    BlockFs *archivo = malloc(sizeof(BlockFs)*size);
    int i;
    for(i=0;i<size;i++){
        recibir(socketFs,&archivo[i].bloque_cp0,sizeof(int32_t),logger);
        recibir(socketFs,&archivo[i].nodo_cp0,sizeof(int32_t),logger);
        recibir(socketFs,&archivo[i].bloque_cp1,sizeof(int32_t),logger);
        recibir(socketFs,&archivo[i].nodo_cp1,sizeof(int32_t),logger);
        recibir(socketFs,&archivo[i].tamanio,sizeof(int32_t),logger);
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

void printTablaEstador(void){
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