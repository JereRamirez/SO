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

char host[20];
char port[20];

int main(int argc, char *argv[]) {
	/********************************************/
	if (argc < 3){
    printf("Ingrese IP a conectarse: ");
    scanf("%s",host);
	printf("Ingrese puerto a conectarse: ");
    scanf("%s",port);
	}
	else{
	strcpy(host,argv[1]);
	strcpy(port,argv[2]);
	}
	/********************************************/

	int socket;

	socket = conectarse_a_yama(host, port);

	//enviar_archivo(socket, "pusheen-sleeping.jpg");

	t_protocolo_almacenamiento_worker* p = recibir_paquete(socket);

	//printf("El ip que me mandaron es: %s\n", strtok(p->ip_port_worker, ":"));
	//printf("El puerto que me mandaron es: %s\n", strtok(NULL, ":"));
	//printf("El nodo que me mandaron es: %d\n", p->nodo);
	//printf("El bloque que me mandaron es: %d\n", p->bloque);
	//printf("Los bytes ocupados son: %ld\n", p->bytes_ocupados);
	//printf("La ruta del archivo es: %s\n", p->archivo_temporal);
	//printf("La ruta del archivo es: %s\n", p->archivo_transformacion);
	//printf("La ruta del archivo es: %s\n", p->archivo_reduccion_local);
	printf("La ruta del archivo es: %s\n", p->archivo_reduccion_global);
	//printf("El encargado que me mandaron es: %d\n", p->encargado);

	printf("Graciaaaaaaaaaaaas\n");

	return EXIT_SUCCESS;
}
