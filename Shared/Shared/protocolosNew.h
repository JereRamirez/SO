/*
 * protocolosNew.h
 *
 *  Created on: 19/10/2017
 *      Author: utnso
 */

#ifndef SHARED_PROTOCOLOSNEW_H_
#define SHARED_PROTOCOLOSNEW_H_

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

//Protocolo de mensajes enviados por YAMA a Master ante una solicitud de transformacion.
typedef struct t_protocolo_transformacion{
    char* ip_port_worker;
    int nodo;
    int bloque;
    long bytes_ocupados;
    char* archivo_temporal;
} __attribute__((packed)) t_protocolo_transformacion;

//Protocolo de mensajes enviados por YAMA a Master ante una solicitud de reduccion local.
typedef struct t_protocolo_reduccion_local{
    char* ip_port_worker;
    int nodo;
    char* archivo_transformacion;
    char* archivo_reduccion_local;
} __attribute__((packed)) t_protocolo_reduccion_local;

//Protocolo de mensajes enviados por YAMA a Master ante una solicitud de reduccion global.
typedef struct t_protocolo_reduccion_global{
    char* ip_port_worker;
    int nodo;
    char* archivo_reduccion_local;
    char* archivo_reduccion_global;
    int encargado; /*sera 1 si es encargado y sera 0 si no lo es*/
} __attribute__((packed)) t_protocolo_reduccion_global;

//Protocolo de mensajes enviados por YAMA a Master ante una solicitud de almacenamiento.
typedef struct t_protocolo_almacenamiento{
    char* ip_port_worker;
    int nodo;
    char* archivo_reduccion_global;
} __attribute__((packed)) t_protocolo_almacenamiento;

//Protocolo de mensajes enviados por Master a Worker para la realizacion de la transformacion.
typedef struct t_protocolo_transformacion_worker{
    int bloque;
    long bytes_ocupados;
    char* archivo_temporal;
} __attribute__((packed)) t_protocolo_transformacion_worker;

//Protocolo de mensajes enviados por Master a Worker para la realizacion de la reduccion local.
typedef struct t_protocolo_reduccion_local_worker{
    char* archivo_transformacion;
    char* archivo_reduccion_local;
} __attribute__((packed)) t_protocolo_reduccion_local_worker;

//Protocolo de mensajes enviados por Master a Worker para la realizacion de la reduccion global.
typedef struct t_protocolo_reduccion_global_worker{
    char* archivo_reduccion_local;
    char* archivo_reduccion_global;
    int encargado; /*sera 1 si es encargado y sera 0 si no lo es*/
} __attribute__((packed)) t_protocolo_reduccion_global_worker;

//Protocolo de mensajes enviados por Master a Worker para la realizacion del almacenamiento final.fsfsa
typedef struct t_protocolo_almacenamiento_worker{
    char* archivo_reduccion_global;
} __attribute__((packed)) t_protocolo_almacenamiento_worker;


enum
{
    PAQ_TRANSFORMACION = 1,
    PAQ_REDUCCION_LOCAL,
    PAQ_REDUCCION_GLOBAL,
    PAQ_ALMACENAMIENTO,
    PAQ_TRANSFORMACION_WORKER,
    PAQ_REDUCCION_WORKER_LOCAL,
    PAQ_REDUCCION_WORKER_GLOBAL,
    PAQ_ALMACENAMIENTO_WORKER
};

//Funcion para enviar respuesta a una solicitud de transformacion. La usa YAMA.
void enviar_paquete_transformacion(socket_t fd, char* ip_port_worker, int nodo, int bloque, long bytes_ocupados ,char* archivo_temporal);

//Funcion para recibir la respuesta de YAMA ante una solicitud de transformacion. Devuelve un puntero a una estructura t_protocolo_transformacion. La usa Master
t_protocolo_transformacion* recibir_paquete_transformacion(socket_t fd);

//Funcion para enviar respuesta a una solicitud de reduccion local. La usa YAMA.
void enviar_paquete_reduccion_local(socket_t fd, char* ip_port_worker, int nodo, char* archivo_transformacion, char* archivo_reduccion_local);

//Funcion para recibir la respuesta de YAMA ante una solicitud de reduccion local. Devuelve un puntero a una estructura t_protocolo_reduccion_local. La usa Master.
t_protocolo_reduccion_local* recibir_paquete_reduccion_local(socket_t fd);

//Funcion para enviar respuesta a una solicitud de reducion global. La usa YAMA.
void enviar_paquete_reduccion_global(socket_t fd, char* ip_port_worker, int nodo, char* archivo_reduccion_local, char* archivo_reduccion_global, int encargado);

//Funcion para recibir la respuesta de YAMA ante una solicitud de reduccion global. Devuelve un puntero a una estructura t_protocolo_reduccion_global. La usa Master.
t_protocolo_reduccion_global* recibir_paquete_reduccion_global(socket_t fd);

//Funcion para enviar respuesta a una solicitud de almacenamiento. La usa YAMA.
void enviar_paquete_almacenamiento(socket_t fd, char* ip_port_worker, int nodo, char* archivo_reduccion_global);

//Funcion para recibir la respuesta de YAMA ante una solicitud de almacenamiento. Devuelve un puntero a una estructura t_protocolo_almacenamiento. La usa Master.
t_protocolo_almacenamiento* recibir_paquete_almacenamiento(socket_t fd);

//Funcion para enviar a worker el bloque sobre el cual aplicar transformacion. La usa Master.
void enviar_paquete_transformacion_worker(socket_t fd, int bloque, long bytes_ocupados, char* archivo_temporal);

//Funcion para recibir informacion sobre que bloques trabajar en la etapa de transformacion. Devuelve un puntero a una estructura t_protocolo_transformacion_worker. La usa worker.
t_protocolo_transformacion_worker* recibir_paquete_transformacion_worker(socket_t fd);

//Funcion para enviar a worker nombres de los archivos sobre los cuales aplicar la reduccion local. La usa Master.
void enviar_paquete_reduccion_local_worker(socket_t fd, char* archivo_transformacion, char* archivo_reduccion_local);

//Funcion para recibir informacion sobre el nombre de los archivos sobre los cuales aplicar la reduccion local. Devuelve un puntero a una estructura t_protocolo_reduccion_local_worker. La usa worker.
t_protocolo_reduccion_local_worker* recibir_paquete_reduccion_local_worker(socket_t fd);

//Funcion para enviar a worker los nombres de los archivos sobre los cuales aplicar la reduccion global. La usa Master.
void enviar_paquete_reduccion_global_worker(socket_t fd, char* archivo_reduccion_local, char* archivo_reduccion_global, int encargado);

//Funcion para recibir informacion sobre el nombre del archivo sobre el cual aplicar la reduccion global. Devuelve un puntero a una estructura t_protocolo_reduccion_global_worker. Lo usa worker.
t_protocolo_reduccion_global_worker* recibir_paquete_reduccion_global_worker(socket_t fd);

//Funcion para enviar a worker el nombre con el cual guardar el archivo final. La usa Master.
void enviar_paquete_almacenamiento_worker(socket_t fd, char* archivo_reduccion_global);

//Funcion para recibir el nombre con el cual guardar el archivo resultante de todas las trnasformaciones y reducciones. Devuelve un puntero a una estructura t_protocolo_almacenamiento_worker. La usa worker.
t_protocolo_almacenamiento_worker* recibir_paquete_almacenamiento_worker(socket_t fd);

//Recibe un protocolo de información. La usa tanto Master como Worker.
void* recibir_paquete(socket_t fd);


#endif /* SHARED_PROTOCOLOSNEW_H_ */
