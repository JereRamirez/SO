/*
 * levantarServidor.c

 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

//Esta funcion deberia pertenecer a una bibilioteca

#include "sockets.h"
#include <sys/socket.h>

#define BACKLOG 5
#define PORT "6667"
#define HOST "localhost"


socket_t main (){
	socket_t listening_socket;
	socket_t socket_cliente;

	listening_socket = crear_listening_socket(HOST, PORT); //que no se de donde saco el host y el port

	listen(listening_socket, BACKLOG);

	struct sockaddr_in hostRemoto;
	socklen_t hostRemotoLen = sizeof(hostRemoto);

	socket_cliente = aceptar_conexion(listening_socket, hostRemoto, hostRemotoLen);

	//Dps de aceptar_conexion va lo que sea que haga el servidor
	/*char package[PACKAGESIZE];
	int status = 1;

	while(status!= 0){
	status = recv(socket_cliente, (void*) package, PACKAGESIZE, 0);
	}*/

	return socket_cliente;
}
