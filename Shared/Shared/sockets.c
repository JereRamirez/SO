/*
 * sockets.c
 *
 *  Created on: 27/9/2017
 *      Author: utnso
 */

#include "sockets.h"
#include <commons/string.h>
#include <commons/error.h>

//Para cliente y servidor
//Configura addrinfo con datos que recibe.
struct addrinfo* configurar_addrinfo(char* host, char*port) {

	struct addrinfo hints, *serverInfo;
	int16_t res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (!strcmp(host, "localhost")) {
		hints.ai_flags = AI_PASSIVE;
		res = getaddrinfo(NULL, port, &hints, &serverInfo);
	} else
		res = getaddrinfo(host, port, &hints, &serverInfo);
	if (res) {
		error_show("Error al hacer getaddrinfo() -> %s \n", gai_strerror(res));
		exit(EXIT_FAILURE);
	}
	return serverInfo;
}

//Para cliente
//Crea y conecta un socket cliente a un servidor -- Invoca a configurar_addrinfo, crea el socket (socket()), y lo conecta (connect()). Devuelve el socket cliente.
socket_t conectar_cliente(char* host, char*port) {
	struct addrinfo* serverInf = configurar_addrinfo(host, port);
	if (serverInf == NULL) {
		exit(EXIT_FAILURE);
	}
	int serverSock = socket(serverInf->ai_family, serverInf->ai_socktype, serverInf->ai_protocol);

	if (serverSock == -1) {
		error_show("No se pudo conectar \n", errno);
		exit(EXIT_FAILURE);
	}
	if (connect(serverSock, serverInf->ai_addr, serverInf->ai_addrlen) == -1) {
		error_show("No se pudo conectar con el server \n", errno);
		close(serverSock);
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(serverInf);

	printf("Conectado al servidor.\n");

	return serverSock;

}

//Para servidor
//Crea un socket que escucha (SERVIDOR) -- Invoca a configurar_addrinfo, crea el socket (socket()) y lo enlaza (bind()). Devuleve el listening socket.
socket_t crear_listening_socket(char* host, char* port) {
	struct addrinfo* serverInf = configurar_addrinfo(host, port);
	if (serverInf == NULL) {
		exit(EXIT_FAILURE);
	}

	int listeningSocket;
	listeningSocket = socket(serverInf->ai_family, serverInf->ai_socktype, serverInf->ai_protocol);
	int activo = 1;
	setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &activo, sizeof(int));
	bind(listeningSocket, serverInf->ai_addr, serverInf->ai_addrlen);

	freeaddrinfo(serverInf);
	return listeningSocket;
}

//Para servidor
//Acepta la conexion de un cliente (accept()).
socket_t aceptar_conexion(socket_t listeningSocket, struct sockaddr_in hostRemoto, socklen_t hostRemotoLen) {
	/*struct sockaddr_in hostRemoto;
	socklen_t hostRemotoLen = sizeof(hostRemoto);*/	/*Lo comento porque en vez de ponerlo aca, lo pongo por fuera de la funcion y se lo paso*/
	socket_t nuevoSocket = accept(listeningSocket,(struct sockaddr*) &hostRemoto, &hostRemotoLen);
	if (nuevoSocket == -1)
		perror("Error al aceptar el socket");
	return nuevoSocket;
}


//Envia un archivo: Transformador/Reductor.
int enviar_archivo(socket_t fd, char* archivo){

	//Manda el nombre del archivo
    send(fd, archivo, 256, MSG_WAITALL);

	//Abre el archivo
    FILE *fp = fopen(archivo, "rb");
    if(fp == NULL){
		printf("Error al abrir archivo\n");
        return 1;
    }

    //Lee la informacion del archivo y la envia
    while(1){

		//Primero lee el archivo en pedazos de 1024 bytes
		unsigned char buff[PACKAGESIZE]={0};
		int nread = fread(buff,1, PACKAGESIZE,fp);
        //printf("Bytes read %d \n", nread);

		//Si la lectura fue existosa se envia la informacion
        if(nread > 0){
            //printf("Sending \n");
            send(fd, buff, nread, MSG_WAITALL);
        }
        if (nread < PACKAGESIZE){
            if (feof(fp)){
				printf("End of file\n");
				printf("File transfer completed for id: %d\n", fd);
			}
			if (ferror(fp)){
                printf("Error de lectura\n");
			}
            break;
        }
    }
    return 0;
}

//Recibe un archivo: Transformador/Reductor.
int recibir_archivo(socket_t fd){

	int bytesReceived = 0;
	char recvBuff[PACKAGESIZE];
    memset(recvBuff, 0, sizeof(recvBuff));

	//Crea el archivo donde se va a guardar la informacion
    FILE *fp;
	char archivo[100];

	//Recibe el nombre del archivo
	recv(fd, archivo, 256, MSG_WAITALL);

	//Abre el archivo
   	fp = fopen(archivo, "ab");
		if(NULL == fp){
			printf("Error abriendo el archivo");
			return 1;
    	}


	//Recibe la informacion en pedazos de 1024 bytes
    while((bytesReceived = recv(fd, recvBuff, PACKAGESIZE, MSG_WAITALL)) > 0){

    	fflush(stdout);
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1, bytesReceived, fp);
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0){
        printf("\n Error de lectura \n");
    }
    printf("\nArchivo OK....Completo\n");
    return 0;
}


/*
char *get_host(){
	char *host = malloc(20);
	printf("Ingrese el ip\n");
	scanf("%s", host);
	return host;
}

char *get_port(){
	char *port = malloc(20);
	printf("Ingrese el puerto\n");
	scanf("%s", port);
	return port;
}
*/

