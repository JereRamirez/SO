/*
 /*
 ============================================================================
 Name        : Master.c
 Author      : NUVUT
 Version     : 1.0
 Copyright   : 
 Description : Process Master
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <Shared/clientes.h>
#include <Shared/sockets.h>
#include <Shared/protocolosNew.h>
#include "libConexion.h"
#include "masterLib.h"
#include <pthread.h>


t_config* config = NULL;
t_log* logger = NULL;
t_list* tabla;

struct ListaWorkers{
	char* ip_worker;
	int32_t puerto_worker;
	char tempFileName[];
	int bitEncargado;
	struct listaWorkers *next;
}listaWorkers;

void devolverDireccion(char* rutina,void* data,int32_t* dimension){
		FILE* archivo = fopen(rutina,"r");
		fseek(archivo,0,SEEK_END);
		dimension = ftell(archivo);
		fseek(archivo,0,SEEK_SET);
		data = malloc(dimension);
		fread(data,1,dimension,archivo);
		fclose(archivo);
}

//ACA ME CONECTO CON EL WORKER Y LE MANDO: el TRANSFORMADOR, bloque en el cual debe hacer la transformacion, bytes ocupados en el bloque y newFileName
//recibo el resultado del Worker y se lo informo a YAMA
char* etapaTransformacion(char* ip_worker, int32_t puerto_worker, int block, char* newTempFileName, int32_t *hs_ms, int32_t bytesOcupados){
	int resultado = 0;
	char* res = "FAIL";

	int32_t socketMaster = cliente(ip_worker, puerto_worker, hs_ms, logger);

	void* data = NULL;
	int32_t dimension = 0;

	devolverDireccion("transformador",data,dimension);

	enviar(socketMaster,dimension,sizeof(int32_t),logger); //Envio la dimension de data como paso previo
	enviar_todo(socketMaster,data,dimension,logger);
	enviar(socketMaster,block,sizeof(int),logger);
	enviar(socketMaster,bytesOcupados,sizeof(int32_t),logger);
	//ver si necesito calcular el sizeof newTempFileName antes
	enviar(socketMaster,newTempFileName,sizeof(char*),logger);
	recibir(socketMaster,&resultado,sizeof(int),logger);

	if(resultado == 1)
		res = "OK";

	return res;
}

//Cuando en un Nodo se ejecutaron todas las operaciones de Transformación,
//inmediatamente pasará a la Etapa de Reducción Local.(ver de poner un contador por cada hilo)

//ACA ME CONECTO CON EL WORKER Y LE MANDO: el REDUCTOR, lista de archivos temporales del Nodo, nombre del nuevo archivo a crear. (ver cuando terminen yama como obtengo los datos de cada worker
	//recibo el resultado del Worker y se lo informo a YAMA
//por cada hilo que creo(por nodo), voy llevando un contador que se va restando cada vez que termina con exito un resultado
char* etapaReduccionLocal(char* ip_worker, int32_t puerto_worker, char tempFile, char* newFileName, int *contador, int32_t hs_ms){
	int resultado = 0;
	char* res = "FAIL";

	int32_t socketMaster = cliente(ip_worker, puerto_worker, hs_ms, logger);

	void* data = NULL;
	int dimension = 0;

	devolverDireccion("reductor",data,dimension);


	enviar(socketMaster,dimension,sizeof(int32_t),logger); //Envio la dimension de data como paso previo
	enviar_todo(socketMaster,data,dimension,logger);
	enviar(socketMaster,tempFile,sizeof(char),logger);
	//ver si necesito calcular el sizeof newFileName antes
	enviar(socketMaster,newFileName,sizeof(newFileName),logger);
	recibir(socketMaster,&resultado,sizeof(int),logger);

		if(resultado == 1)
			res = "OK";

	if( res == "OK")
		contador--;

	return res;
}

//ACA ME CONECTO CON EL WORKER ENCARGADO Y LE MANDO: el REDUCTOR, una lista/estructura de los workers con sus IPs y puertos, lista de archivos temporales de Reduccion Local, ruta donde guardar el resultado
	//recibo el resultado del Worker y se lo informo a YAMA
char* etapaReduccionGlobal(char *ip_worker, int32_t puerto_worker,int32_t hs_ms, struct listaWorkers, char tempListFiles){
	char* resultado = "FAIL";

	int32_t socketMaster = cliente(ip_worker, puerto_worker, hs_ms, logger);

	void* data = NULL;
	int dimension = 0;

	devolverDireccion("reductor",data,dimension);

	enviar_todo(socketMaster,data,dimension,logger);
	enviar(socketMaster,listaWorkers,sizeof(listaWorkers),logger);

return resultado;
}

//ACA ME CONECTO CON EL WORKER ENCARGADO Y SOLICITo: que se conecte al filesystem y le envie el contenido del archivo de Reduccion Global y el nombre y ruta que este debera tener.
	//recibo el resultado del Worker y se lo informo a YAMA
char* etapaAlmacenadoFinal(char *ip_worker, int32_t puerto_worker,int32_t hs_ms, int32_t socketWorker){
	char* resultado = "FAIL";

	int32_t socketMaster = cliente(ip_worker, puerto_worker, hs_ms, logger);

return resultado;
}


void levantarConfig(int32_t *puertoYama, int32_t *handshakeYama,char* ipYama)
{
	config = config_create("master.config");
	if (config == NULL) {
		log_error(logger,"Nunca encontre el archivo de config");
		exit(2);
	}

	*puertoYama = config_has_property(config,"PUERTO_YAMA")?config_get_int_value(config,"PUERTO_YAMA"):-1;
	*handshakeYama = config_has_property(config,"HANDSHAKE_YAMA")?config_get_int_value(config,"HANDSHAKE_YAMA"):-1;
	*ipYama = config_has_property(config,"IP_YAMA")?config_get_string_value(config,"IP_YAMA"):-1;
}

void levantarLog()
{
	logger = log_create("master.log","MASTER",0,LOG_LEVEL_TRACE);
}

void awakening()
{

}

int main(int argc, char *argv[]) {
	char * pathTransformador;
	char * pathReductor;
	char * yamaFs;
	char * pathResultado;
	int32_t puertoYama;
	int32_t handshakeYama;
	char* ipÝama;
	/*inicializacion*/
	/********************************************/
	levantarLog();
	levantarConfig(&puertoYama,&handshakeYama,&ipÝama);
	/********************************************/

	/********************************************/
	if (argc == 5){
		strcpy(pathTransformador, argv[1]);
		strcpy(pathReductor, argv[2]);
		strcpy(yamaFs, argv[3]);
		strcpy(pathResultado, argv[4]);
	}
	else
	{
		log_error(logger,"La cantidad de parametros no es la correcta para inicializar master");
		exit(2);
	}
	/********************************************/

	// aca se conecta con YAMA e incia la primera etapa

	int32_t socketCYAMA = cliente(ipÝama, puertoYama, handshakeYama, logger);
	int32_t msg = strlen(yamaFs);
	solicitud* solicitudes;
	yamaPTH* aux;
	enviar(socketCYAMA,&msg,sizeof(int32_t),logger);
	enviar(socketCYAMA,yamaFs,msg,logger);

	int32_t cantidad;
	recibir(socketCYAMA, &cantidad, sizeof(int32_t),logger);

	solicitudes = malloc(sizeof(solicitud) * cantidad);
	aux = malloc(sizeof(yamaPTH) * cantidad);
	for(int i = 0;i < cantidad;i++)
	{
		recibir(socketCYAMA,&msg,sizeof(int32_t),logger);
		solicitudes[i].Nodo = malloc(msg+1);
		recibir(socketCYAMA, solicitudes[i].Nodo, msg, logger);

		recibir(socketCYAMA,&msg,sizeof(int32_t),logger);
		solicitudes[i].Ip = malloc(msg+1);
		recibir(socketCYAMA, solicitudes[i].Ip, msg, logger);

		recibir(socketCYAMA, &(solicitudes[i].Bloque), sizeof(int32_t), logger);

		recibir(socketCYAMA, &(solicitudes[i].cantidadBytes), sizeof(int32_t), logger);

		recibir(socketCYAMA,&msg,sizeof(int32_t),logger);
		solicitudes[i].archivoTemp = malloc(msg+1);
		recibir(socketCYAMA, solicitudes[i].archivoTemp, msg, logger);

		aux[i].Solicitud = solicitudes[i];
		aux[i].socketYAMA = socketCYAMA;

		pthread_create(aux[i].hilo,NULL,etapaTransformacion,aux[i]);
	}




	return EXIT_SUCCESS;
}
