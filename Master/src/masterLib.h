#include <stdio.h>
#include <pthread.h>

typedef struct {
    int32_t* socketYAMA;
    pthread_t* hilo;
    solicitud Solicitud;
}yamaPTH;

typedef struct {
	char* Nodo;
	char* Ip;
	int Bloque;
	int cantidadBytes;
	char* archivoTemp;
}solicitud;

