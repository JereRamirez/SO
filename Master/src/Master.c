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

int main(void) {
	char* host;
	char* port;
	host = get_host();
	port = get_port();

	int socket;

	socket = conectarse_a_yama(host, port);

	return EXIT_SUCCESS;
}
