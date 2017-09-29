/*
 * clientes.h
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#ifndef CLIENTES_H_
#define CLIENTES_H_

#include "sockets.h"

//Funcion que usa Master, quien sabe a que host y port conectarse

socket_t conectarse_a_worker(char* host, char*port);

//Funcion que usa Master para conectarse a Yama

socket_t conectarse_a_yama(char* host, char*port);

//Funcion que usa Yama para conectarse a FileSystem

socket_t conectarse_a_fsystem(char* host, char*port);

//Funcion que usa FileSystem para conectarse a DataNode

socket_t conectarse_a_datanode(char* host, char*port);

#endif /* CLIENTES_H_ */
