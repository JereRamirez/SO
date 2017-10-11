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
#include <Shared/protocolos.h>

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

	t_paquete_A* p = recibir_paquete(socket);

	printf("El ip que me mandaron es: %s\n", p->ip_worker);
	printf("El puerto que me mandaron es: %s\n", p->port_worker);
	printf("El bloque que me mandaron es: %d\n", p->bloque);
	printf("El ip que me mandaron es: %s\n", p->archivo_temporal);

	printf("Graciaaaaaaaaaaaas\n");

	return EXIT_SUCCESS;
}
