/*
 ============================================================================
 Name        : Master.c
 Author      : NUVUT
 Version     : 1.0
 Copyright   : 
 Description : Process Master
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <Shared/clientes.h>
#include <Shared/sockets.h>
#include <Shared/protocolosNew.h>

char etapaTransformacion(char IP, char port, int block, char newFileName){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER Y LE MANDO: el TRANSFORMADOR, bloque en el cual debe hacer la transformacion, bytes ocupados en el bloque y newFileName
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA

return resultado;
}

char etapaReduccionLocal(char IP, char port, char tempFiles, char newFileName){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER Y LE MANDO: el REDUCTOR, lista de archivos temporales del Nodo newFileName
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA
//por cada hilo que creo, voy llevando un contador que se va restando cada vez que termina con exito un resultado
return resultado;
}

char etapaReduccionGlobal(char IP, char port, char listaWorkers, char tempListFiles){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER ENCARGADO Y LE MANDO: el REDUCTOR, una lista/estructura de los workers con sus IPs y puertos, lista de archivos temporales de Reduccion Local
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA

return resultado;
}

char etapaAlmacenadoFinal(char IP, char port, char ReduGlobalFileName){
	char resultado = 'FAIL';
//ACA ME CONECTO CON EL WORKER ENCARGADO Y SOLICITo: que se conecte al filesystem y le envie el contenido del archivo de Reduccion Global y el nombre y ruta que este debera tener.
	//recibo el resultado del Worker y lo meto en resultado para devolverselo a YAMA

return resultado;
}

char host[20];
char port[20];


int main(int argc, char *argv[]) {
	/********************************************/
	if (argc < 3){
    printf("Ingrese IP a conectarse: ");
    scanf("%s",host);
	printf("Ingrese puerto a conectarse: ");
    scanf("%s",port);
	}
	else{
	strcpy(host,argv[1]);
	strcpy(port,argv[2]);
	}
	/********************************************/

	int socket;

	socket = conectarse_a_yama(host, port);

	//enviar_archivo(socket, "pusheen-sleeping.jpg");

	t_protocolo_almacenamiento_worker* p = recibir_paquete(socket);

	//printf("El ip que me mandaron es: %s\n", strtok(p->ip_port_worker, ":"));
	//printf("El puerto que me mandaron es: %s\n", strtok(NULL, ":"));
	//printf("El nodo que me mandaron es: %d\n", p->nodo);
	//printf("El bloque que me mandaron es: %d\n", p->bloque);
	//printf("Los bytes ocupados son: %ld\n", p->bytes_ocupados);
	//printf("La ruta del archivo es: %s\n", p->archivo_temporal);
	//printf("La ruta del archivo es: %s\n", p->archivo_transformacion);
	//printf("La ruta del archivo es: %s\n", p->archivo_reduccion_local);
	printf("La ruta del archivo es: %s\n", p->archivo_reduccion_global);
	//printf("El encargado que me mandaron es: %d\n", p->encargado);

	printf("Graciaaaaaaaaaaaas\n");

	return EXIT_SUCCESS;
}
