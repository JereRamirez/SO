/*
 * servidor_fork.c
 *
 *  Created on: 14/9/2017
 *      Author: utnso
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PUERTO "6667"
#define BACKLOG 5 //Cantidad de clientes que va a poder escuchar a la vez el servidor
#define PACKAGESIZE 1024 //Tamaño maximo del paquete a enviar

void do_processing (int sock);

int main() {

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// No importa si uso IPv4 o IPv6
	hints.ai_flags = AI_PASSIVE;		// Asigna el address del localhost: 127.0.0.1
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

	getaddrinfo(NULL, PUERTO, &hints, &serverInfo); // Notar que le pasamos NULL como IP, ya que le indicamos que use localhost en AI_PASSIVE

	//Llamamos a la funcion socket()
	int listenningSocket;
	listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	//Mensaje de error si falla la apertura del socket
	if(listenningSocket < 0){
      perror("ERROR opening socket");
      exit(1);
	}

	//Hacemos el bind y mandamos mensaje de error si falla
	if(bind(listenningSocket,serverInfo->ai_addr, serverInfo->ai_addrlen) < 0){
      perror("ERROR on binding");
      exit(1);
	}

	freeaddrinfo(serverInfo); /***********************************/

	//Comenzamos a escuchar a los clientes
	listen(listenningSocket, BACKLOG);

	struct sockaddr_in addr;			// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);

	int pid;

	while(1){

		int socketCliente = accept(listenningSocket, (struct sockaddr *) &addr, &addrlen);

		//Mensaje de error si falla el accept
		if (socketCliente < 0){
			perror("ERROR on accept");
			exit(1);
		}

		//Creamos un proceso hijo
		pid = fork();

		//Mensaje de error si falla el fork
		if (pid < 0) {
			perror("ERROR on fork");
			exit(1);
		}

		if (pid == 0) {
			//Este es el proceso del cliente
			close(listenningSocket);
			do_processing(socketCliente);
			exit(0);
		}
		else {
			close(socketCliente);
		}

	}

	return 0;

}


void do_processing (int socketCliente){

	char package[PACKAGESIZE];
	int status = 1;

	printf("Cliente conectado. Esperando mensajes:\n");

	while (status != 0){
		status = recv(socketCliente, (void*) package, PACKAGESIZE, 0);

		if (status == 0) {
			perror("Coneccion terminada"); //Se cerro la coneccion o se produjo un ERROR al recibir el mensaje
			exit(1);
		}

		if (status != 0) printf("%s", package);

		status = send(socketCliente, "Mensaje recibido", strlen("Mensaje recibido") + 1, 0);

		if (status < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}


	}

}

