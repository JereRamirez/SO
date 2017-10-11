/*
 * sockets.h
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

//Constante que representa el tamaño de los pedazos a recibir de los paquetes
#define PACKAGESIZE 1024

typedef int socket_t;
//typedef char host_t[20];
//typedef char port_t[20];

//Pide el ingreso de un ip

char *get_host();

//Pide el ingreso de un puerto

char *get_port();

//Crea y conecta un socket cliente a un servidor
//Invoca a configurar_addrinfo, crea el socket (socket()), y lo conecta (connect()). Devuelve el socket cliente.

socket_t conectar_cliente(char* host, char*port);

//Crea un socket que escucha (SERVIDOR)
//Invoca a configurar_addrinfo, crea el socket (socket()) y lo enlaza (bind()). Devuleve el listening socket.

socket_t crear_listening_socket(char* host, char* port);

////Acepta la conexion de un cliente (accept()).

socket_t aceptar_conexion(socket_t listeningSocket, struct sockaddr_in hostRemoto, socklen_t hostRemotoLen);


//Envia un archivo: Transformador/Reductor.

int enviar_archivo(socket_t fd, char* archivo);

//Recibe un archivo: Transformador/Reductor.

int recibir_archivo(socket_t fd);


#endif /* SOCKETS_H_ */
