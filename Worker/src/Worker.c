/*
 ============================================================================
 Name        : Worker.c
 Author      : NUVUT
 Version     : 1.0
 Copyright   : 
 Description : Process Worker
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <Shared/servidores.h>

char port[20];


/*En la etapa de Transformacion, c/ hijo de master se conecta al Worker e indica en que bloques emplear la Transformacion, cant de bytes ocupados en ese bloque y en que archivo guardar el resultado
Al terminar el proceso, Worker devuelve confirmacion del resultado*/
char etapaTransformacion(int32_t block, int bOcupados, char file){
 char estado = 'FRACASO';
 int auxestado = 0;

 //Hay que ver que devuelve la funcion que nos da la catedra si no se ejecuta con exitol, PARA EMPEZAR SUPONGO que recibe el bloque donde ejecutar la transformacion, donde guardar el resultado y devuelve 0 en caso de fracaso o 1 en exito
 auxestado = TRANSFORMADOR(block,file);

 if(auxestado == 1){
	 estado = 'EXITO';
 }

 return(estado);
}

int main(int argc, char *argv[]) {
	/********************************************/
	if (argc < 2){
    printf("Ingrese puerto: ");
    scanf("%s",port);
	}
	else
	strcpy(port,argv[1]);
	/********************************************/

	socket_t socket_cliente = levantar_servidor_fork(port);

	return EXIT_SUCCESS;
}
