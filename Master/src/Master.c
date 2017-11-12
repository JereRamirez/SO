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
