/*
 * protocolosNew.c
 *
 *  Created on: 19/10/2017
 *      Author: utnso
 */

#include "protocolosNew.h"
#include "sockets.h"

void enviar_paquete_transformacion(socket_t fd, char* ip_port_worker, int nodo, int bloque, long bytes_ocupados ,char* archivo_temporal){

    int res;
    int codigo = PAQ_TRANSFORMACION;
    size_t ip_port_length = strlen(ip_port_worker);
    size_t archivo_length = strlen(archivo_temporal);

    void* buffer = malloc((5 * sizeof(int)) + sizeof(long) + ip_port_length + archivo_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int), &ip_port_length, sizeof(int));
    memcpy(buffer + (2 * sizeof(int)), ip_port_worker, ip_port_length);
    memcpy(buffer + (2 * sizeof(int)) + ip_port_length, &nodo, sizeof(int));
    memcpy(buffer + (3 * sizeof(int)) + ip_port_length, &bloque, sizeof(int));
    memcpy(buffer + (4 * sizeof(int)) + ip_port_length, &bytes_ocupados, sizeof(long));
    memcpy(buffer + (4 * sizeof(int)) + ip_port_length + sizeof(long), &archivo_length, sizeof(int));
    memcpy(buffer + (5 * sizeof(int)) + ip_port_length + sizeof(long), archivo_temporal, archivo_length);

    res = send(fd, buffer, (5 * sizeof(int)) + sizeof(long) + ip_port_length + archivo_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos");  // aca tiene que ir un logger perror imprime por pantalla lo cual no se quiere hacer
    }

    free(buffer);

}

t_protocolo_transformacion* recibir_paquete_transformacion(socket_t fd){

    size_t ip_port_length;
    size_t archivo_length;
    int bloque;
    int nodo;
    long bytes_ocupados;

    struct t_protocolo_transformacion* miPaquete = malloc(sizeof(t_protocolo_transformacion));

    recv(fd, &ip_port_length, sizeof(int), MSG_WAITALL);
    void* ip_port_worker = malloc(ip_port_length);
    recv(fd, ip_port_worker, ip_port_length, MSG_WAITALL);

    recv(fd, &nodo, sizeof(int), MSG_WAITALL);

    recv(fd, &bloque, sizeof(int), MSG_WAITALL);

    recv(fd, &bytes_ocupados, sizeof(long), MSG_WAITALL);

    recv(fd, &archivo_length, sizeof(int), MSG_WAITALL);
    void* archivo_temporal = malloc(archivo_length);
    recv(fd, archivo_temporal, archivo_length, MSG_WAITALL);

    miPaquete->ip_port_worker = (char *)ip_port_worker;
    miPaquete->nodo = nodo;
    miPaquete->bloque = bloque;
    miPaquete->bytes_ocupados = bytes_ocupados;
    miPaquete->archivo_temporal = (char *)archivo_temporal;

    return miPaquete;

}

void enviar_paquete_reduccion_local(socket_t fd, char* ip_port_worker, int nodo, char* archivo_transformacion, char* archivo_reduccion_local){

    int res;
    int codigo = PAQ_REDUCCION_LOCAL;
    size_t ip_port_length = strlen(ip_port_worker);
    size_t archivo_transformacion_length = strlen(archivo_transformacion);
    size_t archivo_reduccion_local_length = strlen(archivo_reduccion_local);

    void* buffer = malloc((5 * sizeof(int)) + ip_port_length + archivo_transformacion_length + archivo_reduccion_local_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int), &ip_port_length, 4);
    memcpy(buffer + (2 * sizeof(int)) , ip_port_worker, ip_port_length);
    memcpy(buffer + (2 * sizeof(int)) + ip_port_length, &nodo, sizeof(int));
    memcpy(buffer + (3 * sizeof(int)) + ip_port_length, &archivo_transformacion_length, 4);
    memcpy(buffer + (4 * sizeof(int)) + ip_port_length, archivo_transformacion, archivo_transformacion_length);
    memcpy(buffer + (4 * sizeof(int)) + ip_port_length + archivo_transformacion_length, &archivo_reduccion_local_length, 4);
    memcpy(buffer + (5 * sizeof(int)) + ip_port_length + archivo_transformacion_length , archivo_reduccion_local, archivo_reduccion_local_length);

    res = send(fd, buffer, (5 * sizeof(int)) + sizeof(long) + ip_port_length + archivo_transformacion_length + archivo_reduccion_local_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos");
    }

    free(buffer);

}

t_protocolo_reduccion_local* recibir_paquete_reduccion_local(socket_t fd){

    size_t ip_port_length;
    size_t archivo_transformacion_length;
    size_t archivo_reduccion_local_length;
    int nodo;

    struct t_protocolo_reduccion_local* miPaquete = malloc(sizeof(t_protocolo_reduccion_local));

    recv(fd, &ip_port_length, sizeof(int), MSG_WAITALL);
    void* ip_port_worker = malloc(ip_port_length);
    recv(fd, ip_port_worker, ip_port_length, MSG_WAITALL);

    recv(fd, &nodo, sizeof(int), MSG_WAITALL);

    recv(fd, &archivo_transformacion_length, sizeof(int), MSG_WAITALL);
    void* archivo_transformacion = malloc(archivo_transformacion_length+1);
    recv(fd, archivo_transformacion, archivo_transformacion_length, MSG_WAITALL);

    recv(fd, &archivo_reduccion_local_length, sizeof(int), MSG_WAITALL);
    void* archivo_reduccion_local = malloc(archivo_reduccion_local_length+1);
    recv(fd, archivo_reduccion_local, archivo_reduccion_local_length, MSG_WAITALL);

    miPaquete->ip_port_worker = (char *)ip_port_worker;
    miPaquete->nodo = nodo;
    miPaquete->archivo_transformacion = (char *)archivo_transformacion;
    miPaquete->archivo_reduccion_local = (char *)archivo_reduccion_local;

    return miPaquete;

}

void enviar_paquete_reduccion_global(socket_t fd, char* ip_port_worker, int nodo, char* archivo_reduccion_local, char* archivo_reduccion_global, int encargado){

    int res;
    int codigo = PAQ_REDUCCION_GLOBAL;
    size_t ip_port_length = strlen(ip_port_worker);
    size_t archivo_reduccion_local_length = strlen(archivo_reduccion_local);
    size_t archivo_reduccion_global_length = strlen(archivo_reduccion_global);

    void* buffer = malloc((6 * sizeof(int)) + ip_port_length + archivo_reduccion_local_length + archivo_reduccion_global_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int), &ip_port_length, sizeof(int));
    memcpy(buffer + (2 * sizeof(int)) , ip_port_worker , ip_port_length);
    memcpy(buffer + (2 * sizeof(int)) + ip_port_length , &nodo, sizeof(int));
    memcpy(buffer + (3 * sizeof(int)) + ip_port_length , &archivo_reduccion_local_length, 4);
    memcpy(buffer + (4 * sizeof(int)) + ip_port_length , archivo_reduccion_local, archivo_reduccion_local_length);
    memcpy(buffer + (4 * sizeof(int)) + ip_port_length + archivo_reduccion_local_length, &archivo_reduccion_global_length, sizeof(int));
    memcpy(buffer + (5 * sizeof(int)) + ip_port_length + archivo_reduccion_local_length , archivo_reduccion_global, archivo_reduccion_global_length);
    memcpy(buffer + (5 * sizeof(int)) + ip_port_length + archivo_reduccion_local_length + archivo_reduccion_global_length, &encargado, sizeof(int));

    res = send(fd, buffer, (6 * sizeof(int)) + ip_port_length + archivo_reduccion_local_length + archivo_reduccion_global_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos");
    }

    free(buffer);

}

t_protocolo_reduccion_global* recibir_paquete_reduccion_global(socket_t fd){

    size_t ip_port_length;
    size_t archivo_reduccion_local_length;
    size_t archivo_reduccion_global_length;
    int nodo;
    int encargado;

    struct t_protocolo_reduccion_global* miPaquete = malloc(sizeof(t_protocolo_reduccion_global));

    recv(fd, &ip_port_length, sizeof(int), MSG_WAITALL);
    void* ip_port_worker = malloc(ip_port_length);
    recv(fd, ip_port_worker, ip_port_length, MSG_WAITALL);

    recv(fd, &nodo, sizeof(int), MSG_WAITALL);

    recv(fd, &archivo_reduccion_local_length, sizeof(int), MSG_WAITALL);
    void* archivo_reduccion_local = malloc(archivo_reduccion_local_length+1);
    recv(fd, archivo_reduccion_local, archivo_reduccion_local_length, MSG_WAITALL);

    recv(fd, &archivo_reduccion_global_length, sizeof(int), MSG_WAITALL);
    void* archivo_reduccion_global = malloc(archivo_reduccion_global_length+1);
    recv(fd, archivo_reduccion_global, archivo_reduccion_global_length, MSG_WAITALL);

    recv(fd, &encargado, sizeof(int), MSG_WAITALL);

    miPaquete->ip_port_worker = (char *)ip_port_worker;
    miPaquete->nodo = nodo;
    miPaquete->archivo_reduccion_local = (char *)archivo_reduccion_local;
    miPaquete->archivo_reduccion_global = (char *)archivo_reduccion_global;
    miPaquete->encargado = encargado;

    return miPaquete;

}

void enviar_paquete_almacenamiento(socket_t fd, char* ip_port_worker, int nodo, char* archivo_reduccion_global){

    int res;
    int codigo = PAQ_ALMACENAMIENTO;
    size_t ip_port_length = strlen(ip_port_worker);
    size_t archivo_reduccion_global_length = strlen(archivo_reduccion_global);

    void* buffer = malloc((2 * 4) + (2 * sizeof(int)) + ip_port_length + archivo_reduccion_global_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int), &ip_port_length, 4);
    memcpy(buffer + (2 * sizeof(int)) , ip_port_worker, ip_port_length);
    memcpy(buffer + (2 * sizeof(int)) + ip_port_length, &nodo, sizeof(int));
    memcpy(buffer + (3 * sizeof(int)) + ip_port_length, &archivo_reduccion_global_length, 4);
    memcpy(buffer + (4 * sizeof(int)) + ip_port_length, archivo_reduccion_global, archivo_reduccion_global_length);

    res = send(fd, buffer, (4 * sizeof(int)) + ip_port_length + archivo_reduccion_global_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos"); //aca se deberia loggear algo
    }

    free(buffer);

}

t_protocolo_almacenamiento* recibir_paquete_almacenamiento(socket_t fd){

    size_t ip_port_length;
    size_t archivo_reduccion_global_length;
    int nodo;

    struct t_protocolo_almacenamiento* miPaquete = malloc(sizeof(t_protocolo_almacenamiento));

    recv(fd, &ip_port_length, sizeof(int), MSG_WAITALL);
    void* ip_port_worker = malloc(ip_port_length);
    recv(fd, ip_port_worker, ip_port_length, MSG_WAITALL);

    recv(fd, &nodo, sizeof(int), MSG_WAITALL);

    recv(fd, &archivo_reduccion_global_length, sizeof(int), MSG_WAITALL);
    void* archivo_reduccion_global = malloc(archivo_reduccion_global_length+1);
    recv(fd, archivo_reduccion_global, archivo_reduccion_global_length, MSG_WAITALL);

    miPaquete->ip_port_worker = (char *)ip_port_worker;
    miPaquete->nodo = nodo;
    miPaquete->archivo_reduccion_global = (char *)archivo_reduccion_global;

    return miPaquete;

}

void enviar_paquete_transformacion_worker(socket_t fd, int bloque, long bytes_ocupados, char* archivo_temporal){

    int res;
    int codigo = PAQ_TRANSFORMACION_WORKER;
    size_t archivo_length = strlen(archivo_temporal);

    void* buffer = malloc( 4 + (2 * sizeof(int)) + sizeof(long) + archivo_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int), &bloque, sizeof(int));
    memcpy(buffer + (2 * sizeof(int)), &bytes_ocupados, sizeof(long));
    memcpy(buffer + (2 * sizeof(int)) + sizeof(long), &archivo_length, 4);
    memcpy(buffer + (3 * sizeof(int)) + sizeof(long), archivo_temporal, archivo_length);

    res = send(fd, buffer,(3 * sizeof(int)) + sizeof(long) + archivo_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos");
    }

    free(buffer);

}

t_protocolo_transformacion_worker* recibir_paquete_transformacion_worker(socket_t fd){

    size_t archivo_length;
    int bloque;
    long bytes_ocupados;

    struct t_protocolo_transformacion_worker* miPaquete = malloc(sizeof(t_protocolo_transformacion_worker));

    recv(fd, &bloque, sizeof(int), MSG_WAITALL);

    recv(fd, &bytes_ocupados, sizeof(long), MSG_WAITALL);

    recv(fd, &archivo_length, sizeof(int), MSG_WAITALL);
    void* archivo_temporal = malloc(archivo_length);
    recv(fd, archivo_temporal, archivo_length, MSG_WAITALL);

    miPaquete->bloque = bloque;
    miPaquete->bytes_ocupados = bytes_ocupados;
    miPaquete->archivo_temporal = (char *)archivo_temporal;

    return miPaquete;

}

void enviar_paquete_reduccion_local_worker(socket_t fd, char* archivo_transformacion, char* archivo_reduccion_local){

    int res;
    int codigo = PAQ_REDUCCION_WORKER_LOCAL;
    size_t archivo_transformacion_length = strlen(archivo_transformacion);
    size_t archivo_reduccion_local_length = strlen(archivo_reduccion_local);

    void* buffer = malloc( (3 * sizeof(int)) + archivo_transformacion_length + archivo_reduccion_local_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int) , &archivo_transformacion_length , sizeof(int));
    memcpy(buffer + (2 * sizeof(int)) , archivo_transformacion, archivo_transformacion_length);
    memcpy(buffer + (2 * sizeof(int)) + archivo_transformacion_length, &archivo_reduccion_local_length, sizeof(int));
    memcpy(buffer + (3 * sizeof(int)) + archivo_transformacion_length, archivo_reduccion_local, archivo_reduccion_local_length);

    res = send(fd, buffer, (3 * sizeof(int)) + archivo_transformacion_length + archivo_reduccion_local_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos"); // aca tiene que ir un logger perror imprime por pantalla lo cual no se quiere hacer
    }

    free(buffer);

}

t_protocolo_reduccion_local_worker* recibir_paquete_reduccion_local_worker(socket_t fd){

    size_t archivo_transformacion_length;
    size_t archivo_reduccion_local_length;

    struct t_protocolo_reduccion_local_worker* miPaquete = malloc(sizeof(t_protocolo_reduccion_local_worker));

    recv(fd, &archivo_transformacion_length, sizeof(int), MSG_WAITALL);
    void* archivo_transformacion = malloc(archivo_transformacion_length+1);
    recv(fd, archivo_transformacion, archivo_transformacion_length, MSG_WAITALL);

    recv(fd, &archivo_reduccion_local_length, sizeof(int), MSG_WAITALL);
    void* archivo_reduccion_local = malloc(archivo_reduccion_local_length+1);
    recv(fd, archivo_reduccion_local, archivo_reduccion_local_length, MSG_WAITALL);

    miPaquete->archivo_transformacion = (char *)archivo_transformacion;
    miPaquete->archivo_reduccion_local = (char *)archivo_reduccion_local;

    return miPaquete;

}

void enviar_paquete_reduccion_global_worker(socket_t fd, char* archivo_reduccion_local, char* archivo_reduccion_global, int encargado){

    int res;
    int codigo = PAQ_REDUCCION_WORKER_GLOBAL;
    size_t archivo_reduccion_local_length = strlen(archivo_reduccion_local);
    size_t archivo_reduccion_global_length = strlen(archivo_reduccion_global);

    void* buffer = malloc( (4 * sizeof(int)) + archivo_reduccion_local_length + archivo_reduccion_global_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int), &archivo_reduccion_local_length , sizeof(int));
    memcpy(buffer + (2 * sizeof(int)), archivo_reduccion_local, archivo_reduccion_local_length);
    memcpy(buffer + (2 * sizeof(int)) + archivo_reduccion_local_length, &archivo_reduccion_global_length, sizeof(int));
    memcpy(buffer + (3 * sizeof(int)) + archivo_reduccion_local_length, archivo_reduccion_global, archivo_reduccion_global_length);
    memcpy(buffer + (3 * sizeof(int)) + archivo_reduccion_local_length + archivo_reduccion_global_length, &encargado, sizeof(int));

    res = send(fd, buffer, (4 * sizeof(int)) + archivo_reduccion_local_length + archivo_reduccion_global_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos"); // aca tiene que ir un logger perror imprime por pantalla lo cual no se quiere hacer
    }

    free(buffer);

}

t_protocolo_reduccion_global_worker* recibir_paquete_reduccion_global_worker(socket_t fd){

    size_t archivo_reduccion_local_length;
    size_t archivo_reduccion_global_length;
    int encargado;

    struct t_protocolo_reduccion_global_worker* miPaquete = malloc(sizeof(t_protocolo_reduccion_global_worker));

    recv(fd, &archivo_reduccion_local_length, 4, MSG_WAITALL);
    void* archivo_reduccion_local = malloc(archivo_reduccion_local_length+1);
    recv(fd, archivo_reduccion_local, archivo_reduccion_local_length, MSG_WAITALL);

    recv(fd, &archivo_reduccion_global_length, 4, MSG_WAITALL);
    void* archivo_reduccion_global = malloc(archivo_reduccion_global_length+1);
    recv(fd, archivo_reduccion_global, archivo_reduccion_global_length, MSG_WAITALL);

    recv(fd, &encargado, sizeof(int), MSG_WAITALL);

    miPaquete->archivo_reduccion_local = archivo_reduccion_local;
    miPaquete->archivo_reduccion_global = archivo_reduccion_global;
    miPaquete->encargado = encargado;

    return miPaquete;

}

void enviar_paquete_almacenamiento_worker(socket_t fd, char* archivo_reduccion_global){

    int res;
    int codigo = PAQ_ALMACENAMIENTO_WORKER;
    size_t archivo_reduccion_global_length = strlen(archivo_reduccion_global);

    void* buffer = malloc( (2 * sizeof(int)) + archivo_reduccion_global_length);

    memcpy(buffer, &codigo, sizeof(int));
    memcpy(buffer + sizeof(int), &archivo_reduccion_global_length, sizeof(int));
    memcpy(buffer + (2 * sizeof(int)), archivo_reduccion_global, archivo_reduccion_global_length);

    res = send(fd, buffer, (2 * sizeof(int)) + archivo_reduccion_global_length, MSG_WAITALL);

    if (res == -1) {
        perror("No se pudo enviar los datos"); // aca tiene que ir un logger perror imprime por pantalla lo cual no se quiere hacer
    }

    free(buffer);

}

t_protocolo_almacenamiento_worker* recibir_paquete_almacenamiento_worker(socket_t fd){

    size_t archivo_reduccion_global_length;

    struct t_protocolo_almacenamiento_worker* miPaquete = malloc(sizeof(t_protocolo_almacenamiento_worker));

    recv(fd, &archivo_reduccion_global_length, sizeof(int), MSG_WAITALL);
    void* archivo_reduccion_global = malloc(archivo_reduccion_global_length+1);
    recv(fd, archivo_reduccion_global, archivo_reduccion_global_length, MSG_WAITALL);

    miPaquete->archivo_reduccion_global = (char *)archivo_reduccion_global;

    return miPaquete;

}

void* recibir_paquete(socket_t fd) {

    void* paquete;
    int codigo;
    int ret = 0;
    ret = recv(fd, &codigo, sizeof(int), MSG_WAITALL);

    if (ret == 0) {
        return NULL;
    }
    switch(codigo){
        case PAQ_TRANSFORMACION: //No se si esa bien comparar asi o con un strcmp
            paquete = recibir_paquete_transformacion(fd);
            break;
        case PAQ_REDUCCION_LOCAL:
            paquete = recibir_paquete_reduccion_local(fd);
            break;
        case PAQ_REDUCCION_GLOBAL:
            paquete = recibir_paquete_reduccion_global(fd);
            break;
        case PAQ_ALMACENAMIENTO:
            paquete = recibir_paquete_almacenamiento(fd);
            break;
        case PAQ_TRANSFORMACION_WORKER:
            paquete = recibir_paquete_transformacion_worker(fd);
            break;
        case PAQ_REDUCCION_WORKER_LOCAL:
            paquete = recibir_paquete_reduccion_local_worker(fd);
            break;
        case PAQ_REDUCCION_WORKER_GLOBAL:
            paquete = recibir_paquete_reduccion_global_worker(fd);
            break;
        case PAQ_ALMACENAMIENTO_WORKER:
            paquete = recibir_paquete_almacenamiento_worker(fd);
            break;
        default:
            //aca deberiamos loggear algo
            break;
    }

    return paquete;
}

