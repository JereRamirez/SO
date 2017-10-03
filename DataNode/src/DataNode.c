/*
 ============================================================================
 Name        : DataNode.c
 Author      : NUVUT
 Version     : 1.0
 Copyright   : 
 Description : Process DataNode
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <Shared/servidores.h>

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

	return EXIT_SUCCESS;
}
