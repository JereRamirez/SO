/*
 * servidores.c
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#include "servidores.h"


socket_t levantar_servidor(){
	socket_t listening_socket;
	socket_t socket_cliente;

	listening_socket = crear_listening_socket(HOST, PORT); //que no se de donde saco el host y el port
	listen(listening_socket, BACKLOG);

	struct sockaddr_in hostRemoto;
	size_t hostRemotoLen = sizeof(hostRemoto);

	socket_cliente = aceptar_conexion(listening_socket, hostRemoto, hostRemotoLen);

	return socket_cliente;
}

void levantar_servidor_fork(){
	socket_t listening_socket;
	socket_t socket_cliente;

	listening_socket = crear_listening_socket(HOST, PORT); //que no se de donde saco el host y el port
	listen(listening_socket, BACKLOG);

	struct sockaddr_in hostRemoto;
	size_t hostRemotoLen = sizeof(hostRemoto);
	int pid;

	while(1){
		socket_cliente = aceptar_conexion(listening_socket, hostRemoto, hostRemotoLen);

		//Creamos un proceso hijo
		pid = fork();

		//Mensaje de error si falla el fork
		if (pid < 0) {
			perror("ERROR on fork");
			exit(1);
		}

		if (pid == 0) {
			//Este es el proceso del cliente
			close(listening_socket);
			do_processing(socket_cliente);	/*do_processing puede con un recv recibir la funcion que se quiere que haga?*/
			exit(0);
		}
		else {
			close(socket_cliente);
		}
	}
}

void do_processing(socket_t fd){

}
