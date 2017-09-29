/*
 ============================================================================
 Name        : FileSystem.c
 Author      : NUVUT
 Version     : 1.0
 Copyright   : 
 Description : Process yamaFS
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <Shared/servidores.h>

int main(void) {
	levantar_servidor();
	//Deberia levanar un cliente para conectarse con DataNode?
	return EXIT_SUCCESS;
}
