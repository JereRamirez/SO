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
char etapaTransformacion(int32_t puerto, int32_t cantidadConexiones, int32_t hs_wk){
	void* data = NULL;
	int block, resultado;
	int32_t bytesOcupados, dimension;
	char newTempFileName[];

 	 int32_t socketWorker = servidor(puerto, cantidadConexiones,logger);
 	 int32_t socketMaster = aceptarCliente(socketWorker,hs_wk,logger)

	recibir(socketMaster,&dimension,sizeof(int32_t),logger);
	int32_t bytesTransformador = recibir_todo(socketMaster,&data,dimension,logger);
 	recibir(socketMaster,&block,sizeof(int),logger);
 	recibir(socketMaster,bytesOcupados,sizeof(int32_t),logger);
 	recibir(socketMaster,newTempFileName,sizeof(char),logger);
 	enviar(socketMaster,resultado,sizeof(int),logger);

 //Hay que ver que devuelve la funcion que nos da la catedra si no se ejecuta con exitol, PARA EMPEZAR SUPONGO que recibe el bloque donde ejecutar la transformacion, donde guardar el resultado y devuelve 0 en caso de fracaso o 1 en exito
 auxestado = TRANSFORMADOR(block,file);

 if(auxestado == 1){
	 estado = 'EXITO';
 }

 return(estado);
}

char etapaReduLocal(void REDUCTOR(char* file, char new),char path,char newFile){
	char estado = 'FAIL';
	int auxestado = 0;

	auxestado = REDUCTOR(path, newFile); //idem etapa transformación, supongo resultados
	if(auxestado == 1){
		estado = 'OK';
	}
return estado;
//El MASTER debería tener un contador de Workers a los que le pidio realizar la Reducción e ir restado ese contador
//cada vez que recibe una confirmación de éxito.
}

//Ver como me pasa la lista de Workers y sus archivos para los dos parametros de la funcion que faltan
char etapaReduGlobal(void REDUCTOR(char* file, char newFile), ,){
	char estado = 'FAIL';
	int contaux = 0;

	while(listaWorkers != NULL){
		//ver como me conecto con cada worker para realizar el reductor en su archivo
		REDUCTOR(fileWorkers, newFile);
		contaux++;
	}
	if(contaux == cantWorkers){
		estado = 'OK';
	}

return estado;
}

char etapaAlmFinal(char* fileGlobal, char newFile){
	char estado = 'FAIL';
//Ver como me conecto al FileSystem para enviarle el contendio del fileGlobal (primero tengo que crear el nuevo archivo y recien ahi se lo paso al FS?)
//depende como haga el proceso, cambio estado a OK en caso de salir correcto.
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
