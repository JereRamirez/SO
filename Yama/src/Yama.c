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
#include <Shared/protocolosNew.h>

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

	//enviar_paquete_almacenamiento(socket_cliente, "127.0.0.2:2210", 1, "PepitaLaPistolera");
	//enviar_paquete_transformacion_worker(socket_cliente, "127.0.0.2:2210", 2, 12, 1025, "PepitaLaPistolera");
	//enviar_paquete_transformacion_worker(socket_cliente, 12, 1025, "PepitaLaPistolera");
	//enviar_paquete_reduccion_local(socket_cliente, "127.0.0.2:6668", 2, "PepitaLaPistolera", "HolaMeLlamoPusheen");
	//enviar_paquete_reduccion_local_worker(socket_cliente, "PepitaLaPistolera", "HolaMeLlamoPusheen");
	//enviar_paquete_reduccion_global(socket_cliente, "127.0.0.2:6668", 2, "PepitaLaPistolera", "HolaMeLlamoPusheen", 1);
	//enviar_paquete_reduccion_global_worker(socket_cliente, "PepitaLaPistolera", "HolaMeLlamoPusheen", 1);
	//enviar_paquete_almacenamiento(socket_cliente, "127.0.0.2:6668", 2, "PepitaLaPistolera");
	enviar_paquete_almacenamiento_worker(socket_cliente, "PepitaLaPistolera");

	return EXIT_SUCCESS;
}
