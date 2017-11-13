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

t_config* config = NULL;
t_log* logger = NULL;
t_list* tabla;

char etapaTransformacion(char IP, char port, int block, char newFileName){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER Y LE MANDO: el TRANSFORMADOR, bloque en el cual debe hacer la transformacion, bytes ocupados en el bloque y newFileName
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA
return resultado;
}

char etapaReduccionLocal(char IP, char port, char tempFiles, char newFileName){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER Y LE MANDO: el REDUCTOR, lista de archivos temporales del Nodo newFileName
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA
//por cada hilo que creo, voy llevando un contador que se va restando cada vez que termina con exito un resultado
return resultado;
}

char etapaReduccionGlobal(char IP, char port, char listaWorkers, char tempListFiles){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER ENCARGADO Y LE MANDO: el REDUCTOR, una lista/estructura de los workers con sus IPs y puertos, lista de archivos temporales de Reduccion Local
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA

return resultado;
}

char etapaAlmacenadoFinal(char IP, char port, char ReduGlobalFileName){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER ENCARGADO Y SOLICITo: que se conecte al filesystem y le envie el contenido del archivo de Reduccion Global y el nombre y ruta que este debera tener.
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA

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


	return EXIT_SUCCESS;
}
