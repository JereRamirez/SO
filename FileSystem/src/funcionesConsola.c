/*
 * funcionesConsola.c
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#include "funcionesConsola.h"



int lsh_exit(char **args)
{
  return 0;
}

int lsh_format(char **args)
{
	if (args[1] != NULL){
		fprintf(stderr, "lsh: incorrect number of arguments\n");
	} else {
	printf("Estoy andando\n");
	}
	return 1;
}

int lsh_rm_d(char **args)
{
	if (args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rm -d\"\n"); /*puede haber error aca, ojo*/
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia eliminar un directorio\n");
		}
	}
	return 1;
}

int lsh_rm_b(char **args)
{
	if (args[2] == NULL || args[3] == NULL || args[4] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rm -b\"\n"); /*puede haber error aca, ojo*/
	} else {
		if(args[5] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo debería eliminar la copia de un bloque\n");
		}
	}
	return 1;
}

int lsh_rm(char **args)
{
	if(args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rm\"\n");
	} else {
		if(strcmp(args[1], "-d") == 0){
			lsh_rm_d(args);
		} else {
			if(strcmp(args[1], "-b") == 0){
				lsh_rm_b(args);
			} else {
				if(args[2] != NULL){
					fprintf(stderr, "lsh: incorrect number of arguments\n");
				} else {
					printf("Yo deberia eliminar un archivo\n");
				}
			}
		}
	}
	return 1;
}

int lsh_rename(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rename\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia cambiarle el nombre a un archivo o directorio\n");
		}
	}
	return 1;
}

int lsh_mv(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"mv\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia mover un archivo o directorio\n");
		}
	}
	return 1;
}

int lsh_cat(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cat\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia mostrar el contenido del archivo\n");
		}
	}
	return 1;
}

int lsh_mkdir(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"mkdir\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia crear un directorio\n");
		}
	}
	return 1;
}

int lsh_cpfrom(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cpfrom\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia copiar un archivo a yamafs\n");
		}
	}
	return 1;
}

int lsh_cpto(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cpto\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia copiar un archivo local a yamafs\n");
		}
	}
	return 1;
}

int lsh_cpblock(char **args)
{
	if (args[1] == NULL || args[2] == NULL || args[3] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cpblock\"\n");
	} else {
		if(args[4] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo debería crear una copia de un bloque de un archivo en el nodo dado\n");
		}
	}
	return 1;
}

int lsh_md5(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"md5\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia solicitar el MD5 de un archivo en yamafs\n");
		}
	}
	return 1;
}

int lsh_ls(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"ls\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia listar los archivos de un directorio\n");
		}
	}
	return 1;
}

int lsh_info (char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"info\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia mostrar toda la informacion de un archivo\n");
		}
	}
	return 1;
}


