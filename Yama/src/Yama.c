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
#include <Shared/servidores.h>
#include <Shared/sockets.h>
#include <Shared/protocolos.h>

char port[20];

int main(int argc, char *argv[]) {
	/********************************************/
	if (argc < 2){
    printf("Ingrese puerto: ");
    scanf("%s",port);
	}
	else
	strcpy(port,argv[1]);
	/********************************************/

	socket_t socket_cliente = levantar_servidor(port);

	//Deberia levantar un cliente para comunicarse con FS??

	//recibir_archivo(socket_cliente);

	enviar_paquete_A(socket_cliente, "127.0.0.2", "668", 25, "PepitaLaPistolera");

	return EXIT_SUCCESS;
}
