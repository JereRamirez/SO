/*
 * servidores.h
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#ifndef SERVIDORES_H_
#define SERVIDORES_H_

#include "sockets.h"

#define HOST "localhost"
#define BACKLOG 5

//Inicia un servidor.
//Funcion que se usa para DataNode y FileSystem
//El socket que devuelve va a ser argumento de una funcion "do_processing"

socket_t levantar_servidor();

//Inicia un servidor que se forkea.
//Funcion que se usa para los Workers
void levantar_servidor_fork();

//Funcion usada dentro de levantar_servidor_fork.
//Funcion que recive a traves de un socket una funcion y la ejecuta?
void do_processing(socket_t fd);

#endif /* SERVIDORES_H_ */
