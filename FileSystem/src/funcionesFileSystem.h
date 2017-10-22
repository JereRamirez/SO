/*
 * funcionesConsola.h
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#ifndef FUNCIONESFILESYSTEM_H_
#define FUNCIONESFILESYSTEM_H_

#include "sockets.h"
#include <libgen.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <commons/collections/list.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/config.h>
#include <commons/log.h>

#define GETBLOQUE 123
#define SETBLOQUE 321
#define HANDSHAKE_NODO 456

#define CANT_COPIAS_BLOQUE 2
#define TAMANIO_BLOQUE (1024*1024)

#define DIR_CANT_MAX 100

char FILE_DIRECTORIO[100];

t_log* logger;

pthread_t hiloConsola;
pthread_t hiloServer;

/************************************* STRUCTS ******************************************************/

typedef struct{
	int ocupado;
} t_bitmap;


typedef struct {
	char *PUERTO;
} t_config_FS;

t_config_FS config;

typedef struct {
	t_list* nodos;
	t_list* directorios;
	t_list* archivos;
} t_filesystem;

t_filesystem filesystem;

typedef struct {
	int index;
	char* nombre;
	int padre;
} t_directorio;

typedef struct {
	char* nombre;
	size_t tamanio;
	char* tipo;
	int directorio;
	bool disponible;
	int cantBloques;
} t_archivo_info;

typedef struct {
	t_archivo_info* info;
	t_list* bloquesDeDatos; //lista de t_archivo_bloque
} t_archivo;

typedef struct { //estructura que tiene las dos copias del bloque
	int numeroBloque;
	t_list* nodosBloque; //tiene dos t_archivo_nodo_bloque, una por cada copia
	long tamanio;
} t_archivo_bloque;

typedef struct {
	char* nombre;
	char* ip;
	char* puerto;
}t_nodo_info;

typedef struct{
	t_nodo_info* info;
	int numeroBloque;  //numero bloque del nodo
}t_archivo_nodo_bloque;

typedef struct {
	t_nodo_info* info;
	int fd;
	bool conectado;
	t_bitmap* bloques;
	u_int32_t cantBloques;
} t_nodo;

/* FUNCIONES AUXILIARES */

void errorHandler(char* msj);
void crearArchivo(char *path, size_t size);
bool existeArchivo(const char* archivo);
size_t tamanioArchivo(char* archivo);
void limpiarArchivo(char *path);
char *leerArchivoCompleto(char *path);
char* leerArchivo(char *path, size_t tamanio);
void escribirArchivo(char *path, char *datos, size_t tamanio);
void* mapearArchivo(char* archivo);
void desmapearArchivo(char* archMapeado, char* archivo);
float bytes_to_megabytes(size_t bytes);
float bytesToKilobytes(size_t bytes);

/* FUNCIONES PRINCIPALES */

int cargarConfig(char* path);
void startFilesystem(char* flag);
void crearFilesystem();
void iniciarFilesystemLimpio();
void iniciarFilesystemConBackUp();
void cargarBackUp();
void crearServer(char* puerto);
void iniciarServer(void* arg);

/* FUNCIONES DE ARCHIVOS */

t_archivo_info* getInfoArchivo(char* nombre, char* tipo, int dirPadre);
t_archivo_bloque* crearBloqueArchivo(int numero, int tamanio);
t_archivo_nodo_bloque* crearArchivoNodoBloque();
void destruirArchivoNodoBloque(t_archivo_nodo_bloque* anb1);
t_archivo_bloque* buscarBloqueArchivo(t_archivo* archivo, int numeroBloque);
void mostrarInfoCompletaArchivo(t_archivo* archivo);
void mostrarBloquesArchivo(t_list* bloques_de_datos);
void printInfoArchivo(t_archivo_info* info);

/* FUNCIONES DIRECTORIOS */

void printDirectorio(t_directorio* directorio);
int obtenerUltimoIndexDirectorios() ;
int crearDir(t_list* directorios, char* nombre, int padre);
void formatearDirectorios();
t_directorio* buscarDirectorioPorId(t_list* directorios, int id);
t_directorio* buscarDirectorioPorNombre(t_list* directorios, char* nombre, int padre);
int renombrarDirectorio(t_list* directorios, int id, char* nuevo_nombre);
int eliminarDirectorioPorId(t_list* directorios, int id);
int eliminarDirectorioPorNombre(char* nombre, int padre);
void destruirDirectorio(t_directorio* directorio);
int buscarIdDirectorioPorNombre(char* directorio);
bool existeDirectorio(char* nombre, int padre);

/* FUNCIONES CONSOLA */

void mostrarConsola();
void borrarArchivo(char* archivo);
void borrarDirectorio(char* directorio);
void borrarBloque(char* archivo, char* bloque, char* copia);
void renombrar(char* nombreViejo, char* nombreNuevo);
void mover(char* ubicacionVieja, char* ubicacionNueva);
void mostrarContenidoArchivo(char * archivo);
void crearDirectorio(char* directorio);
void copiarFrom(char* origen, char* destino, char* tipo);
void copiarTo(char* origen, char* destino);
void copiarBloqueANodo(char* archivo, char* bloque, char* nodo);
void mostrarMD5(char* archivo);
void mostrarArchivosDelDirectorio(char* directorio);
void mostrarInfoArchivo(char* archivo);

/* Otras funciones */

void informarErrorYLiberarEstructuras(t_config *config_tmp, char *toLog);
t_list* partirArchivoEnBloques(char* archivo);
int cantBloquesNecesarios(char* archivo);
int lenHastaEnter(char* strings);
int procesarMensaje(int fd);
t_nodo* crearNodo(int fd, char* nombre, char* puerto, u_int32_t tamanioData);
int recibirInfoNodo(int fd);
t_bitmap* crearBitmap(u_int32_t tamanio);
void printInfoNodo(t_nodo* nodo);
int cantBloquesLibresNodo(t_nodo* nodo);
t_list* distribuirBloque();
t_nodo* getNodoParaGuardarBloque(t_list* nodos);
t_nodo* getNodoParaGuardarBloqueDistintoANodo(t_list* nodos, t_nodo* nodo0);
int enviarBloqueANodos(t_archivo_bloque* bloqueArchivo, char* bloque);
t_nodo* buscarNodoPorNombre(char* nombre);
int getBloqueLibreNodo(t_nodo* nodo);
int cantBloquesLibresFs();
int cantBloquesLibresNodo(t_nodo* nodo);
int copiarArchivoLocalAlFs(char* nombre, char* tipo, int dirPadre);
void marcarBloqueComoUsado(char* nombre, int numeroBloque);
int guardarBloque(char* data, size_t tamanio, t_archivo_bloque* ab);


#endif /* FUNCIONESFILESYSTEM_H_ */
