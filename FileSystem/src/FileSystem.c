/*
 ============================================================================
 Name        : FileSystem.c
 Author      : NUVUT
 Version     : 1.0
 Copyright   : 
 Description : Process yamaFS
 ============================================================================
 */

#include "funcionesFileSystem.h"

int main(int argc, char *argv[]) {

	puts("******* PROCESO FILESYSTEM *******");

	cargarConfig(argv[1]);

	crearFilesystem();

	crearServer(config.PUERTO);

	pthread_create( &hiloConsola, NULL, (void*)mostrarConsola, NULL);

	pthread_join(hiloConsola, NULL);

	log_destroy(logger);

	return EXIT_SUCCESS;
}
