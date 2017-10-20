#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>
#include <commons/collections/queue.h>
#include <commons/log.h>
struct sockaddr_in initAddr(in_addr_t address,int32_t puerto,t_log *log){
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	// Ordenación de máquina
	my_addr.sin_port = htons(puerto);
	// short, Ordenación de la red
	my_addr.sin_addr.s_addr = address;
	if(my_addr.sin_addr.s_addr == -1){
		log_error(log,"BroadcastError\n");
		exit(1);
	}
	memset(&(my_addr.sin_zero), '\0', 8);
	return my_addr;
}
int32_t enviar(int32_t socketd, void *buf,int32_t bytestoSend,t_log *log){
	int32_t numbytes;
	if (numbytes = send(socketd, buf, bytestoSend, 0) < 0){
		log_error(log,"FALLA AL ENVIAR desde FD: %d",socketd);
		return numbytes;
	}
	return numbytes;
}

int32_t recibir(int32_t socketd, void *buf,int32_t bytestoRecv,t_log *log){
	int32_t numbytes =recv(socketd, buf, bytestoRecv, 0);
	if(numbytes <=0){
		if ((numbytes) <0) {
			log_error(log,"Falla al Recibir");
		}else{
			log_error(log,"DESCONECTADO socket %d\n",socketd);
		}
		log_warning(log,"CERRANDO FD: %d",socketd);
		close(socketd);
	}
	return numbytes;
}
int32_t enviar_todo(int32_t socketd, void *buf,int32_t bytestoSend,t_log *log){
	int32_t numbytes =0;
    int32_t tempSend;
    while (bytestoSend - numbytes >0){
        if (tempSend = send(socketd, buf+numbytes, bytestoSend-numbytes, 0) < 0){
			log_error(log,"FALLA AL ENVIAR UNA PARTE DEL TODO");
            return tempSend;
        }
        numbytes+=tempSend;
        if(tempSend == 0)
            return 0;
    }
	return numbytes;
}

int32_t recibir_todo(int32_t socketd, void *buf,int32_t bytestoRecv,t_log *log){
	int32_t numbytes =recv(socketd, buf, bytestoRecv,MSG_WAITALL);
	if(numbytes <=0){
		if ((numbytes) <0) {
			log_error(log,"Falla al Recibir");
		}else{
			log_error(log,"DESCONECTADO socket %d\n",socketd);
		}
		log_warning(log,"CERRANDO FD: %d",socketd);
		close(socketd);
	}
	return numbytes;
}
int32_t initSocket(t_log *log){
	int32_t sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);// ¡Comprueba que no hay errores!
	if(sockfd ==-1 ){
		log_error(log,"FALLA AL INICIALIZAR SOCKET ---- SALIENDO");
		exit(1);
	}
	return sockfd;
}
void setsocket(int32_t sockfd,int32_t *num,t_log *log){
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, num,sizeof(*num)) == -1) {
		log_error(log,"FALLA AL REALIZAR SETSOCKOPT ---- SALIENDO");
		exit(1);
	}
}
void bindeo(int32_t sockfd,struct sockaddr_in *hostServer,t_log *log){
	if (bind(sockfd, (struct sockaddr *)hostServer, sizeof(*hostServer)) == -1) {
		log_error(log,"FALLA AL REALIZAR BIND ---- SALIENDO");
		exit(1);
	}
}

void makeConnection(int32_t sockfd,struct sockaddr_in *hostServer,int32_t handshake,t_log *log){
	if (connect(sockfd, (struct sockaddr *)hostServer,sizeof(struct sockaddr)) == -1) {
		log_error(log,"FALLA AL CONECTAR --- SALIENDO");
		exit(1);
	}
	int32_t numbytes;
	int32_t aux = handshake;
	if(numbytes= enviar(sockfd,&aux,sizeof(aux),log) <0 ){
		log_error(log,"FALLA AL ENVIAR HANDSHAKE --- SALIENDO");
		exit(1);
	}
	int32_t var;
	log_info(log,"Envio HANDSHAKE");
	if(numbytes = recibir(sockfd,&var,sizeof(var),log) <=0){
		if(var!=1){
			log_error(log,"FALLA HANDSHAKE DISCONNECT AND END");
			close(sockfd);
			exit(1);
		}
	}
}

void escuchando(int32_t sockfd,int32_t cantConnections,t_log *log)
{
	if (listen(sockfd, cantConnections) == -1) {
		log_error(log,"FALLA AL ESCUCHAR ---- SALIENDO ");
		close(sockfd);
		exit(1);
	}
}

void selecteando(int32_t fdmax, fd_set *read_fds){

	if (select(fdmax+1, read_fds, NULL, NULL, NULL) == -1) {
		perror("select");
		exit(1);
	}
}

int32_t servidor(int32_t puerto,int32_t cantidadConexiones,t_log *log){
	int32_t socketServer = initSocket(log);
	int32_t yes=1;
	setsocket(socketServer,&yes,log);
	struct sockaddr_in direccion;
	direccion = initAddr(INADDR_ANY,puerto,log);
	bindeo(socketServer,&direccion,log);
	escuchando(socketServer,cantidadConexiones,log);
	return socketServer;
}

int32_t cliente(char *ip,int32_t puerto,int32_t handshake,t_log *log){
	int32_t sockCliente = initSocket(log);
	struct sockaddr_in direccion = initAddr(inet_addr(ip),puerto,log);
	makeConnection(sockCliente,&direccion,handshake,log);
	return sockCliente;
}

int32_t verificarPaquete(int32_t sockCliente,int32_t handshake,t_log *log){
	int32_t recibido;
	int32_t bytes = recibir(sockCliente,&recibido,sizeof(recibido),log);
	if( bytes >0 && recibido==handshake){
		return 0;
	}else{
		log_error(log,"FALLA -- NO SE ASOCIA -- ");
		return -1;
	}
}
int32_t aceptarCliente(int32_t sockfd, int32_t handshake,t_log *log){
	struct sockaddr_in remoteaddr;
	int32_t newfd;
	int32_t addrlen = sizeof(remoteaddr);

	if ((newfd = accept(sockfd, (struct sockaddr*)&remoteaddr,&addrlen)) == -1)
	{
		log_error(log,"FALLA AL ACEPTAR --- ERROR SOCKET");

		return -1;
	} else {
		log_info(log,"En proceso de ACEPTAR A.. %s",inet_ntoa(remoteaddr.sin_addr));
		if(!verificarPaquete(newfd,handshake,log)){
			log_info(log,"TENGO UNA NUEVA CONEXION %s en SOCKET FD %d", inet_ntoa(remoteaddr.sin_addr),newfd);
			int32_t var = 1;
			enviar(newfd,&var,sizeof(var),log);
			return newfd;
		}
		else{
			log_error(log,"HandShake FAIL");
			close(newfd);
		}
		return -1;
	}

}
