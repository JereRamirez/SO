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

	cargarConfig(argv[2]);

	startFilesystem(argv[1]);

	log_destroy(logger);

	return EXIT_SUCCESS;
}
