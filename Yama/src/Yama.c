/*
 ============================================================================
 Name        : Yama.c
 Author      : NUVUT
 Version     : 1.0
 Copyright   : 
 Description : Process Yama
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <Shared/protocolosNew.h>
#include <Shared/libConexion.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>

t_config* config = NULL;
t_log* logger = NULL;
t_list* tabla;

enum {
	SENTINELA, //este estado jamas se usa pero nos indica si una estructura esta vacia
	EN_ESPERA = 1,
	EN_PROCESO,
	FINALIZADO_OK,
	ERROR
}estado;

enum {
	TRANSFORMACION = 1,
	REDUCCION_LOCAL,
	REDUCCION_GLOBAL,
	ALMACENAMIENTO
}etapa;

typedef struct {
	int32_t job;
	int32_t master;
	char nodo[20];
	int32_t bloque;
	estado ESTADO = 0;
	char temp[50];
	etapa ETAPA;
}tablaEst;


void levantarConfig(int32_t *puertoServerYama,char* ipFileSystem, int32_t *puertoFileSystem, int32_t *handshakeFileSystem)
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
}

void levantarLog()
{
	logger = log_create("yama.log","YAMA",0,LOG_LEVEL_TRACE);
}

char* AsignarNombreUnico(int semilla){
	return "nombreunico";
}

void creacionBasica(int32_t masterid, tablaEst EstructuraT,
		t_list* tabla) {
	EstructuraT->job = 1;
	EstructuraT->master = masterid;
	EstructuraT->nodo = NULL;
	EstructuraT->bloque = NULL;
	EstructuraT->ETAPA = 1;
	EstructuraT->temp = NULL;
	EstructuraT->ESTADO = 1;
	list_add(tabla, *EstructuraT);
}

int find_masterid(t_list *list,int masterid){
int i,cantElem = list_size(list);
tablaEst *aux;
for (i = 0; i < cantElem; i++)
{
	aux= list_get(list,i);
	if(aux->master == masterid)
		return i;
}
return -1;
}
void guardarNodosDisponibles(t_list * nodosFilesystem, t_list * nodosLocales)
{
	list_add_all(nodosFilesystem,nodosLocales);
}
int main(int argc, char *argv[]) {
	int32_t puerto;
	char* ip_fs;
	int32_t puerto_fs;
	int32_t hs_fs;
	int32_t masterid;
	t_list * nodosDisponibles;
	tablaEst EstructuraT;
	/*inicializacion*/
	/********************************************/
	levantarLog();
	levantarConfig(&port,&ip_fs, &puerto_fs, &hs_fs);
	tabla = list_create();
	/********************************************/

	/* lo primero que hago es conectarme con filesystem y obtener la lista completa de nodos a usar*/
	socket_t socket_cliente = cliente(ip_fs, puerto_fs, hs_fs, logger);
	int tamanio;
	recibir(socket_cliente,tamanio,sizeof(tamanio),logger);
	socket_t socket_server = servidor(puerto,0,logger);


	//TODO: aca un master me estaria enviando un archivo
	masterid = 0; // este valor harcodeado deberia venir desde master
	//primero me fijo si este master esta en la lista con un job
	if (list_is_empty(tabla)) {
		creacionBasica(masterid, &EstructuraT, tabla);
	}
	tablaEst busqueda = find_masterid(tabla, masterid);

	switch (busqueda.ESTADO) {
		case SENTINELA:
			//esto significaria que master no existe por lo que lo creamos
			creacionBasica(masterid, &busqueda, tabla);
			break;
		case EN_ESPERA:

			break;
		case EN_PROCESO:

			break;
		case FINALIZADO_OK:

			break;
		case ERROR:

			break;
		default:
			break;
	}


	socket_t socket_cliente = cliente(ip_fs,puerto_fs,hs_fs,logger);

	if (socket_cliente == 1)
	{
		return -1;
	}
	// esto es lo que se le manda a FileSystem el cual me va a responder con los nodos
	//enviar(socket_cliente,/*aca va el buffer que va a ccontener sobre q*/,sizeof(/*aca va el buffer*/),logger)
	//recibir(socket_cliente,/*aca lo que voy a recibir posiblemente haya que hacer una estructura en protocolos*/,/*sizeof(esa estructura),logger)
	//recibir_archivo(socket_cliente);


	return EXIT_SUCCESS;
}

//enviar_paquete_almacenamiento(socket_cliente, "127.0.0.2:2210", 1, "PepitaLaPistolera");
//enviar_paquete_transformacion_worker(socket_cliente, "127.0.0.2:2210", 2, 12, 1025, "PepitaLaPistolera");
//enviar_paquete_transformacion_worker(socket_cliente, 12, 1025, "PepitaLaPistolera");
//enviar_paquete_reduccion_local(socket_cliente, "127.0.0.2:6668", 2, "PepitaLaPistolera", "HolaMeLlamoPusheen");
//enviar_paquete_reduccion_local_worker(socket_cliente, "PepitaLaPistolera", "HolaMeLlamoPusheen");
//enviar_paquete_reduccion_global(socket_cliente, "127.0.0.2:6668", 2, "PepitaLaPistolera", "HolaMeLlamoPusheen", 1);
//enviar_paquete_reduccion_global_worker(socket_cliente, "PepitaLaPistolera", "HolaMeLlamoPusheen", 1);
//enviar_paquete_almacenamiento(socket_cliente, "127.0.0.2:6668", 2, "PepitaLaPistolera");
//enviar_paquete_almacenamiento_worker(socket_cliente, "PepitaLaPistolera");
