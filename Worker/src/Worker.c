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
#include "libConexion.h"

char port[20];


/*En la etapa de Transformacion, c/ hijo de master se conecta al Worker e indica en que bloques emplear la Transformacion, cant de bytes ocupados en ese bloque y en que archivo guardar el resultado
Al terminar el proceso, Worker devuelve confirmacion del resultado*/
void etapaTransformacion(int32_t puerto, int32_t cantidadConexiones, int32_t hs_wk){
	void* data = NULL;
	int block, resultado = 0;
	int32_t bytesOcupados, dimension;
	char newTempFileName[];

 	 int32_t socketWorker = servidor(puerto, cantidadConexiones,logger);
 	 int32_t socketMaster = aceptarCliente(socketWorker,hs_wk,logger);

	recibir(socketMaster,&dimension,sizeof(int32_t),logger);
	int32_t bytesTransformador = recibir_todo(socketMaster,&data,dimension,logger);
 	recibir(socketMaster,&block,sizeof(int),logger);
 	recibir(socketMaster,bytesOcupados,sizeof(int32_t),logger);
 	recibir(socketMaster,newTempFileName,sizeof(char),logger);


 	enviar(socketMaster,resultado,sizeof(int),logger);
 return;
}

void etapaReduLocal(int32_t puerto, int32_t cantidadConexiones, int32_t hs_wk){
	void* data = NULL;
	int block, resultado = 0;
	int32_t bytesOcupados, dimension;
	char tempFile[], newFileName[];

	 int32_t socketWorker = servidor(puerto, cantidadConexiones,logger);
	 int32_t socketMaster = aceptarCliente(socketWorker,hs_wk,logger);


	recibir(socketMaster,&dimension,sizeof(int32_t),logger);
	int32_t bytesLocal = recibir_todo(socketMaster,&data,dimension,logger);
	recibir(socketMaster,&tempFile,sizeof(char),logger);
	 //ver si necesito calcular el sizeof newFileName antes
	recibir(socketMaster,newFileName,sizeof(newFileName),logger);


	enviar(socketMaster,resultado,sizeof(int),logger);
 return;
//El MASTER debería tener un contador de Workers a los que le pidio realizar la Reducción e ir restado ese contador
//cada vez que recibe una confirmación de éxito.
}

//Ver como me pasa la lista de Workers y sus archivos para los dos parametros de la funcion que faltan
void etapaReduGlobal(int32_t puerto, int32_t cantidadConexiones, int32_t hs_wk){
	char estado = 'FAIL';
	int contaux = 0;
	void* data = NULL;
	int block, resultado = 0;
	int32_t bytesOcupados, dimension;
	char tempFile[], newFileName[];

	 int32_t socketWorker = servidor(puerto, cantidadConexiones,logger);
	 int32_t socketMaster = aceptarCliente(socketWorker,hs_wk,logger);


	recibir(socketMaster,&dimension,sizeof(int32_t),logger);
	int32_t bytesGlobal = recibir_todo(socketMaster,&data,dimension,logger);
	recibir(socketMaster,&listaWorkers,sizeof(listaWorkers),logger); //ver como me lo pasa yama para pasarselo al worker
	recibir(socketMaster,&newPathFile,sizeof(newPathFile),logger);	//ver como paso el path del nuevo file


//ver como uso la funcion que me pasa el worker
	while(listaWorkers != NULL){

		REDUCTOR(fileWorkers, newFile);
		contaux++;
	}
	if(contaux == cantWorkers){
		estado = 'OK';
	}
	enviar(socketMaster,resultado,sizeof(int),logger);

return ;
}

//Me conecto al FS y le mando el archivo de red global
char etapaAlmFinal(int32_t puerto, int32_t cantidadConexiones, int32_t hs_wk){
	char estado = 'FAIL';
	int resultado = 0;

	int32_t socketWorker = servidor(puerto, cantidadConexiones,logger);
	int32_t socketMaster = aceptarCliente(socketWorker,hs_wk,logger);

	recibir(socketMaster,&ip_fs,sizeof(ip_fs),logger);
	recibir(socketMaster,&puerto_fs,sizeof(int32_t),logger);
	recibir(socketMaster,&newPathFile,sizeof(newPathfile),logger);

	int32_t socketFS = cliente(ip_fs, puerto_fs, hs_wk, logger);


	enviar(socketMaster,resultado,sizeof(int),logger);
return estado;
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
