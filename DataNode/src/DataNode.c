#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#define MB 1024*1024
#define GETBLOQUE 123
#define SETBLOQUE 321
#include <commons/collections/list.h>
#include "libConexion.h"
#define DEFAULTCONFIG "nodo.config"

void* getBloque(int32_t block,void* dataBin){
    void* dataBlock = malloc(MB);
    memcpy(dataBlock,dataBin+block*MB,MB);
    return dataBlock;
}
void setBloque(int32_t block, void*data,int32_t sizeData,void* dataBin){
    memcpy(dataBin+block*MB,data,sizeData);
    free(data);
}
void keysNodo(t_config *config,t_log *log){
    bool error=false;
    if(!config_has_property(config,"IP_FILESYSTEM") || !config_has_property(config,"PUERTO_FILESYSTEM") || !config_has_property(config,"NOMBRE_NODO") || !config_has_property(config,"PUERTO_WORKER") || !config_has_property(config,"RUTA_DATABIN")){
        log_error(log,"CONFIG INCOMPLETO");
        exit(2);
    }
    log_info(log,"CON TAGS CORRECTOS -- ROGUEMOS QUE LA DATA ESTE CORRECTA");
}
void* mapFile(char* pathFile,int32_t *bloques,t_log *log){
    int32_t fd = open(pathFile,O_RDWR);
    struct stat sbuf;
    fstat(fd,&sbuf);

    if(fd <0){
        log_error(log,"Error al Abrir el Archivo -- EXIT 8");
        exit(8);
    }
    void* data = mmap((caddr_t)0,sbuf.st_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
    if(data == -1){
        log_error(log,"ERROR AL MAPEAR BITMAP -- EXIT 3");
        exit(3);
    }
    if(msync(data,sbuf.st_size,MS_ASYNC)==-1)
    {
        log_error(log,"ERROR AL SINCRONIZAR BITMAP -- EXIT 4");
        exit(4);
    }
    int cantidadBloques = sbuf.st_size /1024/1024;
    *bloques = cantidadBloques;
    log_info(log,"El archivo contiene %i",cantidadBloques);
    return data;
}
void *loadConfig(char* path,int32_t *client,int32_t *cantBloques,t_log *log){
    t_config *aux = config_create(path);
    if(aux ==NULL) {
        log_error(log,"FALLA AL ABRIR CONFIG");
        exit(1);
    }
    keysNodo(aux,log);

    char *data = mapFile(config_get_string_value(aux,"RUTA_DATABIN"),cantBloques,log);
    *client = cliente(config_get_string_value(aux,"IP_FILESYSTEM"),config_get_int_value(aux,"PUERTO_FILESYSTEM"),10100,log);
    char* nombreNodo = config_get_string_value(aux,"NOMBRE_NODO");
    int32_t lenNombre = strlen(nombreNodo)+1;
    int32_t puertoWorker = config_get_int_value(aux,"PUERTO_WORKER");
    enviar(*client,(void*)cantBloques,sizeof(int32_t),log);//Falta checker de desconexiones y demas
    enviar(*client,(void*)&lenNombre,sizeof(int32_t),log);
    enviar(*client,(void *)nombreNodo,lenNombre,log);
    enviar(*client,(void *)&puertoWorker,sizeof(int32_t),log);
    config_destroy(aux);
    return data;
}

int main(int argc, char* argv[]) {
    t_log *log;
    void* dataBin;
    log = log_create("dataNode.log","DATANODE",false,LOG_LEVEL_TRACE);
    int32_t socket,cantBloques;
    if(log == NULL)
        exit(-1);
    switch(argc){
        case 1:
            dataBin =loadConfig(DEFAULTCONFIG,&socket,&cantBloques,log);
            break;
        case 2:
            dataBin = loadConfig(argv[1],&socket,&cantBloques,log);
            break;
        default:
            exit(-1);
    }
    int32_t datosRecibido=1,msg;

    for(;datosRecibido >0;datosRecibido = recibido(socket,&msg,sizeof(msg))){
        switch(msg){
            case GETBLOQUE:

                break;
            case SETBLOQUE:

                break;
            default:
                log_error(log,"Mensaje recibido \"%d\" INVALIDO",msg);

        }
    }

    return 0;
}