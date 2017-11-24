
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libConexion.h"
#include <commons/config.h>
#include <commons/log.h>
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

typedef struct {
	int32_t job;
	int32_t master;
	int32_t	nodo;
	int32_t bloque;
	int32_t estado;
	int32_t etapa;
	char* archTemp;
}tablaEst;


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
	return "nombreunico";
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
	data=RQ_ARCH_FS;
	pthread_mutex_lock(&filesystem_m);
	enviar(socketFs,&data,sizeof(int32_t),logger);
	data=strlen(archivoYamafs);
	enviar(socketFs,&data,sizeof(int32_t),logger);
	enviar(socketFs,archivoYamafs,data,logger);
	bytesRecibidos= recibir(socketMaster,&data,sizeof(data),logger);//chek conexion?
	BlockFs* bloques=arrayBlockFs(data,logger);
	pthread_mutex_unlock(&filesystem_m);


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
	int32_t *masterAux;
	while(true){
		masterAux = malloc(sizeof(int32_t));
		log_info(logger,"ESPERANDO CONEXION MASTER");
		*masterAux = aceptarCliente(socket_server,hs_ms,logger);

		if(*masterAux>0){
			log_info(logger,"NUEVO MASTER CONECTADO FD=%d",*masterAux);
			pthread_mutex_lock(&listaMaster_m);
			list_add(masters,masterAux);
			pthread_mutex_unlock(&listaMaster_m);
			masterAux=NULL;
		}
		else{
			log_error(logger,"NO ES MAESTRO, es solo un ENTRENADOR");
			free(masterAux);
		}
	}


	return EXIT_SUCCESS;
}
