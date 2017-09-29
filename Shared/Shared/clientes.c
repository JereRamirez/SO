/*
 * clientes.c
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#include "clientes.h"
#include <stdio.h>

//Funcion que usa Master, quien sabe a que host y port conectarse (porque se lo dijo Yama)
socket_t conectarse_a_worker(char* host, char*port){
	socket_t server_socket;
	server_socket = conectar_cliente(host, port);
	return server_socket;
}

//Funcion que usa Master para conectarse a Yama
socket_t conectarse_a_yama(char* host, char*port){
	socket_t server_socket;
	server_socket = conectar_cliente(host, port);
	return server_socket;
}

//Funcion que usa Yama para conectarse a FileSystem
socket_t conectarse_a_fsystem(char* host, char*port){
	socket_t server_socket;
	server_socket = conectar_cliente(host, port);
	return server_socket;
}

//Funcion que usa FileSystem para conectarse a DataNode
socket_t conectarse_a_datanode(char* host, char*port){
	socket_t server_socket;
	server_socket = conectar_cliente(host, port);
	return server_socket;
}

char *get_host(){
	char *host = malloc(20);
	printf("Ingrese el ip\n");
	scanf("%s", host);
	printf("IP = %s \n", host);
	return host;
}

char *get_port(){
	char *port = malloc(20);
	printf("Ingrese el puerto\n");
	scanf("%s", port);
	printf("Puerto = %s \n", port);
	return port;
}
