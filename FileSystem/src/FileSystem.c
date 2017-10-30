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

	logger = log_create("Filesystem.log", "FileSystem", false, LOG_LEVEL_TRACE);

	startFilesystem(argv[1]);

	log_destroy(logger);

	return EXIT_SUCCESS;
}
