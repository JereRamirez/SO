
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libConexion.h"
#include <commons/config.h>
#include <commons/log.h>
enum {
    SENTINELA, //este estado jamas se usa pero nos indica si una estructura esta vacia
    EN_ESPERA = 1,
    EN_PROCESO,
    FINALIZADO_OK,
    ERROR=-1
};

enum {
    TRANSFORMACION = 1,
    REDUCCION_LOCAL,
    REDUCCION_GLOBAL,
    ALMACENAMIENTO,

};
#include "yamaLib.h"
#include <commons/collections/list.h>

t_config* config = NULL;
t_log* logger = NULL;
t_list* tabla;
t_list* masters;
char* algoritmoPlanificador;
int32_t socketFs;
pthread_mutex_t listaMaster_m;
pthread_mutex_t filesystem_m;
pthread_mutex_t planificador;



void levantarConfig(int32_t *puertoServerYama,char* ipFileSystem, int32_t *puertoFileSystem, int32_t *handshakeFileSystem,int32_t* handshakeMasters)
{
	config = config_create("yama.config");
	if (config == NULL) {
		log_error(logger,"Nunca encontre el archivo de config");
		exit(2);
	}

	*puertoServerYama = config_has_property(config,"PUERTO_SERVER_YAMA")?config_get_int_value(config,"PUERTO_SERVER_YAMA"):-1;
	*ipFileSystem = config_has_property(config,"IP_FILESYSTEM")?config_get_string_value(config,"IP_FILESYSTEM"):-1;
	*puertoFileSystem = config_has_property(config,"PUERTO_FILESYSTEM")?config_get_int_value(config,"PUERTO_FILESYSTEM"):-1;
	*handshakeFileSystem = config_has_property(config,"HANDSHAKE_FILESYSTEM")?config_get_int_value(config,"HANDSHAKE_FILESYSTEM"):-1;
	*handshakeMasters = config_has_property(config,"HANDSHAKE_MASTER")?config_get_int_value(config,"HANDSHAKE_MASTER"):-1;

}

void levantarLog()
{
	logger = log_create("yama.log","YAMA",0,LOG_LEVEL_TRACE);
}

char* AsignarNombreUnico(int semilla){
	return "nombreunico";//TROLL
}

void guardarNodosDisponibles(t_list * nodosFilesystem, t_list * nodosLocales)
{
	list_add_all(nodosFilesystem,nodosLocales);
}

void manejaMasterPlanificador(int32_t socketMaster){
	int32_t data;
	int32_t bytesRecibidos= recibir(socketMaster,&data,sizeof(data),logger);

	char* archivoYamafs = malloc(data);
	bytesRecibidos= recibir(socketMaster,archivoYamafs,(data),logger);//RECIBO NOMBRE ARCHIVO FS//chek conexion?
    pthread_mutex_lock(&filesystem_m);
    t_list* Nodos= arrayNodos(socketFs,logger);
    pthread_mutex_unlock(&filesystem_m);

    data=RQ_ARCH_FS;
    pthread_mutex_lock(&filesystem_m);
	enviar(socketFs,&data,sizeof(int32_t),logger);
	data=strlen(archivoYamafs);
	enviar(socketFs,&data,sizeof(int32_t),logger);
	enviar(socketFs,archivoYamafs,data,logger);
	bytesRecibidos= recibir(socketMaster,&data,sizeof(data),logger);//chek conexion?
	BlockFs* bloques=arrayBlockFs(socketFs,data,logger);
	pthread_mutex_unlock(&filesystem_m);
    //INICIO PRE PLANIFICACION

    //FIN PRE PLANIFICACION
    //ENVIO MASTER

    //ESPERO ESTADOS ESPERO RECEIVE POR CANTIDAD DE WORKERS // SI FALLA REPLANIFICO ALGUNO O TERMINO SI NO SE PUEDE

    //PREPLANIFICO REDU LOCAL
    //ORIGINO NOMBRES RANDM de FTEmp Y MANDO INSTRUCCIONES A LOS NODOS PAR LAS RECUDDIONES LOCALES
    //nodo, archivosTemp en NOdo, ArchivoRedLocal
    //FIN PREPLANIFIC REDU LOCAL
    //ENVIO MASTER

    //Espero Estado, si falla algo, a la goma

    //PREPLANIFICO REDU GLOBAL
    //ORIGINO NOMBRES RANDM de FTEmp Y MANDO INSTRUCCIONES A LOS NODOS PAR LAS RECUDDIONES GLOBALES
    //nodo, ARCHIVOLOCALNODO, NODO AL QUE CONECTARTE(IP Y PUERTO), SI SOY RECEPTOR

    //FIN PREPLANIFIC REDU GLOBAL
    //ENVIO MASTER

    //ESPERO ESTADOS SI EXPPLOTA JUDIO MENOS -- PUKY JODETE-- ADRO ES TURKO KBOOM

    //LOAD TO FS
    //MANDO A NODO FINAL A GUARDAR FILE DE REDUCCION GLOBAL CON EL NOMBRE ESPECIFICO en FS
    //ESPERO RESOLUCION Y CIERRO CONEXION CON MASTER


}


int main(int argc, char *argv[]) {
	int32_t puerto;
	char* ip_fs;
	int32_t datosRecibidos;
	int32_t puerto_fs;
	int32_t hs_fs;
	int32_t hs_ms;
	t_list* nodosDisponibles;
	tablaEst EstructuraT;
	int32_t cantMasters=0;
	/*inicializacion*/
	/********************************************/
	levantarLog();
	levantarConfig(&puerto,ip_fs, &puerto_fs, &hs_fs,&hs_ms);
	masters = list_create();
	tabla = list_create();
	/********************************************/

	/* lo primero que hago es conectarme con filesystem y obtener la lista completa de nodos a usar*/
    socketFs= cliente(ip_fs, puerto_fs, hs_fs, logger);
	int32_t socket_server = servidor(puerto,99,logger);
	MasterPTH aux;
    aux.state = true;
    while(true){
		aux.socketMaster = malloc(sizeof(int32_t));
		log_info(logger,"ESPERANDO CONEXION MASTER");
		*aux.socketMaster = aceptarCliente(socket_server,hs_ms,logger);

		if(*aux.socketMaster>0){
			log_info(logger,"NUEVO MASTER CONECTADO FD=%d",*aux.socketMaster);
			pthread_mutex_lock(&listaMaster_m);
			list_add(masters,&aux);
            aux.hilo=malloc(sizeof(pthread_t));
            pthread_create(aux.hilo,NULL,manejaMasterPlanificador,aux.socketMaster);
			pthread_mutex_unlock(&listaMaster_m);
			aux.hilo=aux.socketMaster=NULL;
		}
		else{
			log_error(logger,"NO ES MAESTRO, es solo un ENTRENADOR");
			free(aux.socketMaster);
		}
	}


	return EXIT_SUCCESS;
}
