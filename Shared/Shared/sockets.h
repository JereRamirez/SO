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

//Primer paquete que recibe Master por parte de YAMA
typedef struct t_paquete_A{
	char* ip_worker;
	char* port_worker;
	int bloque;
	char* archivo_temporal;
} t_paquete_A;

//Segundo paquete que recibe Master por parte de YAMA
typedef struct t_paquete_B{
	char* ip_worker;
	char* port_worker;
	char* archivo_temporal;
	char* archivo_local;
} t_paquete_B;

//Tercer paquete que recibe Master por parte de YAMA
typedef struct t_paquete_C{
	char* ip_worker;
	char* port_worker;
	char* archivo_local;
	char* ip_encargado;
	char* port_encargado;
	char* ruta;
} t_paquete_C;

//Cuarto paquete que recibe Master por parte de YAMA
typedef struct t_paquete_D{
	char* ip_worker;
	char* port_worker;
	char* archivo_global;
} t_paquete_D;

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

//Recibe un paquete de información.

void* recibir_paquete(socket_t fd);

//Recibe un paquete del tipo A. Devuelve el paquete recibido.

t_paquete_A* recibir_paquete_A(socket_t fd);

//Recibe un paquete del tipo B. Devuelve el paquete recibido.

t_paquete_B* recibir_paquete_B(socket_t fd);

//Recibe un paquete del tipo C. Devuelve el paquete recibido.

t_paquete_C* recibir_paquete_C(socket_t fd);

//Recibe un paquete del tipo D. Devuelve el paquete recibido.

t_paquete_D* recibir_paquete_D(socket_t fd);

//Envia un paquete del tipo A.

void enviar_paquete_A(socket_t fd, char* ip_worker, char* port_worker, int bloque, char* archivo_temporal);

//Envia un paquete del tipo B.

void enviar_paquete_B(socket_t fd, char* ip_worker, char* port_worker, char* archivo_temporal, char* archivo_local);

//Envia un paquete del tipo C.

void enviar_paquete_C(socket_t fd, char* ip_worker, char* port_worker, char* archivo_local, char* ip_encargado,
		char* port_encargado, char* ruta);

//Envia un paquete del tipo D.

void enviar_paquete_D(socket_t fd, char* ip_worker, char* port_worker, char* archivo_global);

//Envia un archivo: Transformador/Reductor.

int enviar_archivo(socket_t fd, char* archivo);

//Recibe un archivo: Transformador/Reductor.

int recibir_archivo(socket_t fd);



#endif /* SOCKETS_H_ */
