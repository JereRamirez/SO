#include <stdio.h>
#include <pthread.h>

typedef struct {
    int32_t* socketYAMA;
    pthread_t* hilo;
    solicitud Solicitud;
    etapas Etapa;
    result Result;
    int encargado;
}yamaPTH;

typedef struct {
	char* Nodo;
	char* Ip;
	int puerto;
	int Bloque;
	int cantidadBytes;
	char* archivoTempTrans;
	char* archivoTempReduLocal;
}solicitud;

typedef enum{
	TRANSFORMA = 1,
	REDULOCAL,
	REDUGLOBAL,
	ALMACENA
}etapas;

typedef enum{
	FAIL,
	OK
}result;
