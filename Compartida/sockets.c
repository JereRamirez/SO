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
socket_t aceptar_conexion(socket_t listeningSocket, struct sockaddr_in hostRemoto, size_t hostRemotoLen) {
	/*struct sockaddr_in hostRemoto;
	size_t hostRemotoLen = sizeof(hostRemoto);*/	/*Lo comento porque en vez de ponerlo aca, lo pongo por fuera de la funcion y se lo paso*/
	socket_t nuevoSocket = accept(listeningSocket,(struct sockaddr*) &hostRemoto, &hostRemotoLen);
	if (nuevoSocket == -1)
		perror("Error al aceptar el socket");
	return nuevoSocket;
}

//Para servidor o cliente
//Recibe un paquete de información.
void* recibir_paquete(socket_t fd) {

	void* paquete;
	int codigo;
	int ret = 0;
	ret = recv(fd, &codigo, sizeof(int), MSG_WAITALL);

	if (ret == 0) {
		return NULL;
	}
	switch(codigo){
		case 1: //No se si esa bien comparar asi o con un strcmp
			paquete = recibir_paquete_A(fd);
			break;
		case 2:
			paquete = recibir_paquete_B(fd);
			break;
		case 3:
			paquete = recibir_paquete_C(fd);
			break;
		case 4:
			paquete = recibir_paquete_D(fd);
			break;
	}

	return paquete;
}

//Usado por la funcion recibir_paquete()
//Recibe un paquete del tipo A. Devuelve el paquete recibido.
t_paquete_A* recibir_paquete_A(socket_t fd) {

	t_paquete_A* paquete_recibido = malloc(sizeof(t_paquete_A));
	recv(fd, &paquete_recibido->ip_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->port_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->bloque, sizeof(int), MSG_WAITALL);
	recv(fd, &paquete_recibido->archivo_temporal, sizeof(char*), MSG_WAITALL);
	return paquete_recibido;
}

//Usado por la funcion recibir_paquete()
//Recibe un paquete del tipo B. Devuelve el paquete recibido.
t_paquete_B* recibir_paquete_B(socket_t fd) {

	t_paquete_B* paquete_recibido = malloc(sizeof(t_paquete_B));
	recv(fd, &paquete_recibido->ip_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->port_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->archivo_temporal, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->archivo_local, sizeof(char*), MSG_WAITALL);
	return paquete_recibido;
}

//Usado por la funcion recibir_paquete()
//Recibe un paquete del tipo C. Devuelve el paquete recibido.
t_paquete_C* recibir_paquete_C(socket_t fd) {

	t_paquete_C* paquete_recibido = malloc(sizeof(t_paquete_C));
	recv(fd, &paquete_recibido->ip_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->port_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->archivo_local, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->ip_encargado, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->port_encargado, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->ruta, sizeof(char*), MSG_WAITALL);
	return paquete_recibido;
}

//Usado por la funcion recibir_paquete()
//Recibe un paquete del tipo D. Devuelve el paquete recibido.
t_paquete_D* recibir_paquete_D(socket_t fd) {

	t_paquete_D* paquete_recibido = malloc(sizeof(t_paquete_D));
	recv(fd, &paquete_recibido->ip_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->port_worker, sizeof(char*), MSG_WAITALL);
	recv(fd, &paquete_recibido->archivo_global, sizeof(char*), MSG_WAITALL);
	return paquete_recibido;
}

//Envia un paquete del tipo A.
void enviar_paquete_A(socket_t fd, char* ip_worker, char* port_worker, int bloque, char* archivo_temporal){

	int res;
	int codigo = 1;
	int tamanioPaquete = (3 * sizeof(char*)) + (sizeof(int)) + (sizeof(codigo));

	void* buffer = malloc(tamanioPaquete);
	memcpy(buffer, &codigo, sizeof(codigo));
	memcpy(buffer + (sizeof(codigo)), &ip_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (sizeof(char*)), &port_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (2 * sizeof(char*)), &bloque, sizeof(int));
	memcpy(buffer + (sizeof(codigo)) + (2 * sizeof(char*)) + (sizeof(int)), &archivo_temporal, sizeof(char*));

	res = send(fd, buffer, tamanioPaquete, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}
	free(buffer);
}

//Envia un paquete del tipo B.
void enviar_paquete_B(socket_t fd, char* ip_worker, char* port_worker, char* archivo_temporal, char* archivo_local){

	int res;
	int codigo = 2;
	int tamanioPaquete = (4 * sizeof(char*)) + (sizeof(codigo));

	void* buffer = malloc(tamanioPaquete);
	memcpy(buffer, &codigo, sizeof(codigo));
	memcpy(buffer + (sizeof(codigo)), &ip_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (sizeof(char*)), &port_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (2 * sizeof(char*)), &archivo_temporal, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (3 * sizeof(char*)), &archivo_local, sizeof(char*));

	res = send(fd, buffer, tamanioPaquete, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}
	free(buffer);
}

//Envia un paquete del tipo C.
void enviar_paquete_C(socket_t fd, char* ip_worker, char* port_worker, char* archivo_local, char* ip_encargado,
		char* port_encargado, char* ruta){

	int res;
	int codigo = 3;
	int tamanioPaquete = (6 * sizeof(char*)) + (sizeof(codigo));

	void* buffer = malloc(tamanioPaquete);
	memcpy(buffer, &codigo, sizeof(codigo));
	memcpy(buffer + (sizeof(codigo)), &ip_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (sizeof(char*)), &port_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (2 * sizeof(char*)), &archivo_local, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (3 * sizeof(char*)), &ip_encargado, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (4 * sizeof(char*)), &port_encargado, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (5 * sizeof(char*)), &ruta, sizeof(char*));

	res = send(fd, buffer, tamanioPaquete, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}
	free(buffer);
}

//Envia un paquete del tipo D.
void enviar_paquete_D(socket_t fd, char* ip_worker, char* port_worker, char* archivo_global){

	int res;
	int codigo = 4;
	int tamanioPaquete = (3 * sizeof(char*)) + (sizeof(codigo));

	void* buffer = malloc(tamanioPaquete);
	memcpy(buffer, &codigo, sizeof(codigo));
	memcpy(buffer + (sizeof(codigo)), &ip_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (sizeof(char*)), &port_worker, sizeof(char*));
	memcpy(buffer + (sizeof(codigo)) + (2 * sizeof(char*)), &archivo_global, sizeof(char*));

	res = send(fd, buffer, tamanioPaquete, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}
	free(buffer);
}

//Envia un archivo: Transformador/Reductor.
int enviar_archivo(socket_t fd, char* archivo){

	//Manda el nombre del archivo
    send(fd, archivo, 256, MSG_WAITALL);

	//Abre el archivo
    FILE *fp = fopen(archivo, "rb");
    if(fp == NULL){
		printf("Error al abrir archivo");
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


