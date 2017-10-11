/*
 * protocolos.c
 *
 *  Created on: 10/10/2017
 *      Author: utnso
 */

#include "protocolos.h"
#include "sockets.h"

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
t_paquete_A* recibir_paquete_A(socket_t fd){

	size_t ip_length;
	size_t port_length;
	size_t archivo_length;
	int bloque;

	struct t_paquete_A* miPaquete = malloc(sizeof(t_paquete_A));

	recv(fd, &ip_length, 4, MSG_WAITALL);
	void* ip_worker = malloc(ip_length);
	recv(fd, ip_worker, ip_length, MSG_WAITALL);

	recv(fd, &port_length, 4, MSG_WAITALL);
	void* port_worker = malloc(port_length);
	recv(fd, port_worker, port_length, MSG_WAITALL);

	recv(fd, &bloque, sizeof(int), MSG_WAITALL);

	recv(fd, &archivo_length, 4, MSG_WAITALL);
	void* archivo_temporal = malloc(archivo_length);
	recv(fd, archivo_temporal, archivo_length, MSG_WAITALL);

	miPaquete->ip_worker = ip_worker;
	miPaquete->port_worker = port_worker;
	miPaquete->bloque = bloque;
	miPaquete->archivo_temporal = archivo_temporal;

	return miPaquete;

}

//Usado por la funcion recibir_paquete()
//Recibe un paquete del tipo B. Devuelve el paquete recibido.
t_paquete_B* recibir_paquete_B(socket_t fd){

	size_t ip_length;
	size_t port_length;
	size_t temporal_length;
	size_t local_length;

	struct t_paquete_B* miPaquete = malloc(sizeof(t_paquete_B));

	recv(fd, &ip_length, 4, MSG_WAITALL);
	void* ip_worker = malloc(ip_length);
	recv(fd, ip_worker, ip_length, MSG_WAITALL);

	recv(fd, &port_length, 4, MSG_WAITALL);
	void* port_worker = malloc(port_length);
	recv(fd, port_worker, port_length, MSG_WAITALL);

	recv(fd, &temporal_length, 4, MSG_WAITALL);
	void* archivo_temporal = malloc(temporal_length);
	recv(fd, archivo_temporal, temporal_length, MSG_WAITALL);

	recv(fd, &local_length, 4, MSG_WAITALL);
	void* archivo_local = malloc(local_length);
	recv(fd, archivo_local, local_length, MSG_WAITALL);

	miPaquete->ip_worker = ip_worker;
	miPaquete->port_worker = port_worker;
	miPaquete->archivo_temporal = archivo_temporal;
	miPaquete->archivo_local = archivo_local;

	return miPaquete;

}

//Usado por la funcion recibir_paquete()
//Recibe un paquete del tipo C. Devuelve el paquete recibido.
t_paquete_C* recibir_paquete_C(socket_t fd){

	size_t ip_length;
	size_t port_length;
	size_t archivo_length;
	size_t ip_encargado_length;
	size_t port_encargado_length;
	size_t ruta_length;

	struct t_paquete_C* miPaquete = malloc(sizeof(t_paquete_C));

	recv(fd, &ip_length, 4, MSG_WAITALL);
	void* ip_worker = malloc(ip_length);
	recv(fd, ip_worker, ip_length, MSG_WAITALL);

	recv(fd, &port_length, 4, MSG_WAITALL);
	void* port_worker = malloc(port_length);
	recv(fd, port_worker, port_length, MSG_WAITALL);

	recv(fd, &archivo_length, 4, MSG_WAITALL);
	void* archivo_local = malloc(archivo_length);
	recv(fd, archivo_local, archivo_length, MSG_WAITALL);

	recv(fd, &ip_encargado_length, 4, MSG_WAITALL);
	void* ip_encargado = malloc(ip_encargado_length);
	recv(fd, ip_encargado, ip_encargado_length, MSG_WAITALL);

	recv(fd, &port_encargado_length, 4, MSG_WAITALL);
	void* port_encargado = malloc(port_encargado_length);
	recv(fd, port_encargado, port_encargado_length, MSG_WAITALL);

	recv(fd, &ruta_length, 4, MSG_WAITALL);
	void* ruta = malloc(ruta_length);
	recv(fd, ruta, ruta_length, MSG_WAITALL);

	miPaquete->ip_worker = ip_worker;
	miPaquete->port_worker = port_worker;
	miPaquete->archivo_local = archivo_local;
	miPaquete->ip_encargado = ip_encargado;
	miPaquete->port_encargado = port_encargado;
	miPaquete->ruta = ruta;

	return miPaquete;

}

//Usado por la funcion recibir_paquete()
//Recibe un paquete del tipo D. Devuelve el paquete recibido.
t_paquete_D* recibir_paquete_D(socket_t fd){

	size_t ip_length;
	size_t port_length;
	size_t archivo_length;

	struct t_paquete_D* miPaquete = malloc(sizeof(t_paquete_D));

	recv(fd, &ip_length, 4, MSG_WAITALL);
	void* ip_worker = malloc(ip_length);
	recv(fd, ip_worker, ip_length, MSG_WAITALL);

	recv(fd, &port_length, 4, MSG_WAITALL);
	void* port_worker = malloc(port_length);
	recv(fd, port_worker, port_length, MSG_WAITALL);

	recv(fd, &archivo_length, 4, MSG_WAITALL);
	void* archivo_global = malloc(archivo_length);
	recv(fd, archivo_global, archivo_length, MSG_WAITALL);

	miPaquete->ip_worker = ip_worker;
	miPaquete->port_worker = port_worker;
	miPaquete->archivo_global = archivo_global;

	return miPaquete;

}

//Envia un paquete del tipo A.
void enviar_paquete_A(socket_t fd, char* ip_worker, char* port_worker, int bloque, char* archivo_temporal){

	int res;
	int codigo = 1;
	size_t ip_length = strlen(ip_worker);
	size_t port_length = strlen(port_worker);
	size_t archivo_length = strlen(archivo_temporal);

	void* buffer = malloc((3 * 4) + (2 * sizeof(int)) + ip_length + port_length + archivo_length);

	memcpy(buffer, &codigo, sizeof(int));
	memcpy(buffer + sizeof(int), &ip_length, 4);
	memcpy(buffer + sizeof(int) + 4, ip_worker, ip_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length, &port_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4, port_worker, port_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length, &bloque, sizeof(int));
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + sizeof(int), &archivo_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + sizeof(int) + 4, archivo_temporal, archivo_length);

	res = send(fd, buffer, (3 * 4) + (2 * sizeof(int)) + ip_length + port_length + archivo_length, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}

	free(buffer);

}

//Envia un paquete del tipo B.
void enviar_paquete_B(socket_t fd, char* ip_worker, char* port_worker, char* archivo_temporal, char* archivo_local){

	int res;
	int codigo = 2;
	size_t ip_length = strlen(ip_worker);
	size_t port_length = strlen(port_worker);
	size_t temporal_length = strlen(archivo_temporal);
	size_t local_length = strlen(archivo_local);

	void* buffer = malloc((4 * 4) + (sizeof(int)) + ip_length + port_length + temporal_length + local_length);

	memcpy(buffer, &codigo, sizeof(int));
	memcpy(buffer + sizeof(int), &ip_length, 4);
	memcpy(buffer + sizeof(int) + 4, ip_worker, ip_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length, &port_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4, port_worker, port_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length, &temporal_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4, archivo_temporal, temporal_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + temporal_length, &local_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + temporal_length + 4, archivo_local, local_length);

	res = send(fd, buffer, (4 * 4) + (sizeof(int)) + ip_length + port_length + temporal_length + local_length, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}

	free(buffer);
}

//Envia un paquete del tipo C.
void enviar_paquete_C(socket_t fd, char* ip_worker, char* port_worker, char* archivo_local, char* ip_encargado, char* port_encargado, char* ruta){

	int res;
	int codigo = 3;
	size_t ip_length = strlen(ip_worker);
	size_t port_length = strlen(port_worker);
	size_t archivo_length = strlen(archivo_local);
	size_t ip_encargado_length = strlen(ip_encargado);
	size_t port_encargado_length = strlen(port_encargado);
	size_t ruta_length = strlen(ruta);

	void* buffer = malloc((6 * 4) + (sizeof(int)) + ip_length + port_length + archivo_length + ip_encargado_length + port_encargado_length + ruta_length);

	memcpy(buffer, &codigo, sizeof(int));
	memcpy(buffer + sizeof(int), &ip_length, 4);
	memcpy(buffer + sizeof(int) + 4, ip_worker, ip_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length, &port_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4, port_worker, port_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length, &archivo_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4, archivo_local, archivo_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + archivo_length, &ip_encargado_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + archivo_length + 4, ip_encargado, ip_encargado_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + archivo_length + 4 + ip_encargado_length, &port_encargado_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + archivo_length + 4 + ip_encargado_length + 4, port_encargado, port_encargado_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + archivo_length + 4 + ip_encargado_length + 4 + port_encargado_length, &ruta_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4 + archivo_length + 4 + ip_encargado_length + 4 + port_encargado_length + 4, ruta, ruta_length);

	res = send(fd, buffer, (6 * 4) + (sizeof(int)) + ip_length + port_length + archivo_length + ip_encargado_length + port_encargado_length + ruta_length, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}

	free(buffer);

}

//Envia un paquete del tipo D.
void enviar_paquete_D(socket_t fd, char* ip_worker, char* port_worker, char* archivo_global){

	int res;
	int codigo = 4;
	size_t ip_length = strlen(ip_worker);
	size_t port_length = strlen(port_worker);
	size_t archivo_length = strlen(archivo_global);

	void* buffer = malloc((3 * 4) + (sizeof(int)) + ip_length + port_length + archivo_length);

	memcpy(buffer, &codigo, sizeof(int));
	memcpy(buffer + sizeof(int), &ip_length, 4);
	memcpy(buffer + sizeof(int) + 4, ip_worker, ip_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length, &port_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4, port_worker, port_length);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length, &archivo_length, 4);
	memcpy(buffer + sizeof(int) + 4 + ip_length + 4 + port_length + 4, archivo_global, archivo_length);

	res = send(fd, buffer, (3 * 4) + (sizeof(int)) + ip_length + port_length + archivo_length, MSG_WAITALL);

	if (res == -1) {
		perror("No se pudo enviar los datos");
	}

	free(buffer);

}
