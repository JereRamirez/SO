/*
 * protocolos.h
 *
 *  Created on: 10/10/2017
 *      Author: utnso
 */

#ifndef SHARED_PROTOCOLOS_H_
#define SHARED_PROTOCOLOS_H_

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
#include "sockets.h"

//Primer paquete que recibe Master por parte de YAMA
typedef struct t_paquete_A{
	char* ip_worker;
	char* port_worker;
	int bloque;
	char* archivo_temporal;
} __attribute__((packed)) t_paquete_A;

//Segundo paquete que recibe Master por parte de YAMA
typedef struct t_paquete_B{
	char* ip_worker;
	char* port_worker;
	char* archivo_temporal;
	char* archivo_local;
} __attribute__((packed)) t_paquete_B;

//Tercer paquete que recibe Master por parte de YAMA
typedef struct t_paquete_C{
	char* ip_worker;
	char* port_worker;
	char* archivo_local;
	char* ip_encargado;
	char* port_encargado;
	char* ruta;
} __attribute__((packed)) t_paquete_C;

//Cuarto paquete que recibe Master por parte de YAMA
typedef struct t_paquete_D{
	char* ip_worker;
	char* port_worker;
	char* archivo_global;
} __attribute__((packed)) t_paquete_D;

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


#endif /* SHARED_PROTOCOLOS_H_ */
