/*
 * funcionesConsola.c
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#include "funcionesFileSystem.h"

/* INICIO FUNCIONES PRINCIPALES */

void startFilesystem(char* flag){
	if(!strncmp(flag, "--clean", 7)){
		iniciarFilesystemLimpio();
	}else{
		iniciarFilesystemConBackUp();
	}
}

void iniciarFilesystemLimpio(){

	crearFilesystem();

	formatearDirectorios();

	crearServer(PUERTO);

	pthread_create(&hiloConsola, NULL, (void*)mostrarConsola, NULL);

	pthread_join(hiloConsola, NULL);
}

void iniciarFilesystemConBackUp(){

	crearFilesystem();

	cargarBackUp();

	crearServer(PUERTO);

	pthread_create(&hiloConsola, NULL, (void*)mostrarConsola, NULL);

	pthread_join(hiloConsola, NULL);

}

void cargarBackUp(){
	return;
}

/* FIN FUNCIONES PRINCIPALES */

/* INICIO FUNCIONES AUXILIARES */

void errorHandler(char* msj){
	do{perror(msj);exit(EXIT_FAILURE);} while(0);
}

bool existeArchivo(const char* archivo) {
	bool rs = true;

	FILE* f = NULL;
	f = fopen(archivo, "r");
	if (f != NULL) {
		fclose(f);
		rs = true;
	} else
		rs = false;

	return rs;
}

void crearArchivo(char *path, size_t size) {

	FILE *f = fopen(path, "wb");

	fseek(f, size - 1, SEEK_SET);

	fputc('\n', f);

	fclose(f);
}

// Devuelve el tamaño del archivo

size_t tamanioArchivo(char* archivo) {
	struct stat st;
	stat(archivo, &st);
	return st.st_size;
}


// Borra tod0 el contenido de un archivo

void limpiarArchivo(char *path) {

	FILE *f = fopen(path, "wb");

	fclose(f);
}

// Mapea un archivo en memoria y lo devuelve

void* mapearArchivo(char* archivo) {

	char *archMapeado;
	int fd;
	struct stat sb;
	size_t tamanio;

	fd = open(archivo, O_RDWR);
	if (fd == -1)
		errorHandler("Error al abrir archivo");

	if (fstat(fd, &sb) == -1)
		errorHandler("Error al hacer fstat");

	tamanio = sb.st_size;
	archMapeado = mmap(NULL, tamanio, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_NORESERVE, fd, 0);

	if (archMapeado == MAP_FAILED)
		errorHandler("Error al ejecutar mmap");

	return archMapeado;
}

//Desmapea un archivo de memoria

void desmapearArchivo(char* archMapeado, char* archivo) {
	munmap(archMapeado, tamanioArchivo(archivo));
}

float bytesToKilobytes(size_t bytes){
	return bytes / (1024 + 0.0);
}

float bytesToMegabytes(size_t bytes){
	return bytes / ((1024*1024) + 0.0);
}

void destroySplit(char** split){
	int i = 0;
	while (split[i] != NULL) {
		freeNull(split[i]);
		i++;
	}

	freeNull(split);
}

/* FIN FUNCIONES AUXILIARES */

/* INICIO FUNCIONES CONSOLA */

void mostrarConsola() {
	char* linea;
	char** lineas;
	while(1) {
		linea = readline(">");

		if(linea)
			add_history(linea);

		if(!strncmp(linea, "exit", 4)) {
			free(linea);
			break;
		}

		if(!strncmp(linea, "format", 6)) {
			puts("Formateando FS \n");
		}

		lineas = string_split(linea, " ");

		if(!strncmp(linea, "rm", 2)) {

			if(!strncmp(linea, "rm -d", 5)) {
				borrarDirectorio(lineas[2]);
			} else if(!strncmp(linea, "rm -b", 5)) {
				borrarBloque(lineas[2], lineas[3], lineas[4]);
			} else {
				borrarArchivo(lineas[1]);
			}
		}

		if(!strncmp(linea, "rename", 6)) {
			renombrar(lineas[1], lineas[2]);
		}

		if(!strncmp(linea, "mv", 2)) {
			mover(lineas[1], lineas[2]);
		}

		if(!strncmp(linea, "cat", 3)) {
			mostrarContenidoArchivo(lineas[1]);
		}

		if(!strncmp(linea, "mkdir", 5)) {
			crearDirectorio(lineas[1]);
		}

		if(!strncmp(linea, "cpfrom", 6)) {
			copiarFrom(lineas[1], lineas[2], lineas[3]);
		}

		if(!strncmp(linea, "cpto", 4)) {
			copiarTo(lineas[1], lineas[2]);
		}

		if(!strncmp(linea, "cpblock", 7)) {
			copiarBloqueANodo(lineas[1], lineas[2],lineas[3]);
		}

		if(!strncmp(linea, "md5", 3)) {
			mostrarMD5(lineas[1]);
		}

		if(!strncmp(linea, "ls", 2)) {
			mostrarArchivosDelDirectorio(lineas[1]);
		}

		if(!strncmp(linea, "info", 4)) {
			mostrarInfoArchivo(lineas[1]);
		}

		freeNull(linea);
		destroySplit(lineas);
	}
}

void borrarArchivo(char* archivo){
	if(archivo == NULL){
		printf("Falta especificar el archivo a borrar\n");
	}else{
		char* nombre = basename(string_duplicate(archivo));
		int dirId = dirGetIndex(dirname(string_duplicate(archivo)));
		if(existeArchivoEnFS(nombre, dirId)){
			borrarArchivoFs(nombre, dirId);
			puts("Archivo borrado exitosamente");
		}else{
			puts("El archivo no existe");
		}
	}
}

void borrarDirectorio(char* directorio){
	if(directorio == NULL){
		printf("Falta especificar el directorio a borrar\n");
	}else{
		int idDirPadre = dirGetIndex(dirname(string_duplicate(directorio)));
		char* nombreDir = basename(string_duplicate(directorio));
		int idDir = dirGetIndex(directorio);
		if (existeDirectorio(nombreDir, idDirPadre)) {
			if(!tieneArchivosDir(idDir)){
				printf("ID a borrar: %d\n", idDir);
				eliminarDirectorioPorId(filesystem.directorios, idDir);
				printf("El directorio fue borrado correctamente\n");
			}else{
				puts("No se puede borrar el directorio ya que contiene archivos");
			}

		}else{
			printf("El directorio no existe\n");
		}
	}
}

void borrarBloque(char* archivo, char* bloque, char* copia){
	if(archivo == NULL || bloque == NULL || copia == NULL){
		printf("Falta especificar archivo,bloque o copia\n");
	}else{
		printf("Borrando la copia: %s del bloque: %s del archivo: %s \n", copia, bloque, archivo );
	}
}

void renombrar(char* nombreViejo, char* nombreNuevo){
	if(nombreViejo == NULL || nombreNuevo == NULL){
		printf("Falta especificar nombre actual o nombre deseado\n");
	}else{
		char* nombre = basename(string_duplicate(nombreViejo));
		int dirId = dirGetIndex(dirname(string_duplicate(nombreViejo)));
		if(string_contains(nombre, ".") && existeArchivoEnFS(nombre, dirId)){
			renombrarArchivo(nombreViejo, nombreNuevo);
		}else if(existeDirectorio(nombre, dirId)){
			renombrarDirectorio(filesystem.directorios, dirGetIndex(nombreViejo), nombreNuevo);
		}else{
			puts("No se pudo renombrar el archivo o directorio");
		}
	}
}

void mover(char* ubicacionVieja, char* ubicacionNueva){
	if(ubicacionVieja == NULL || ubicacionNueva == NULL){
		printf("Falta especificar ubicación actual o ubicación deseada\n");
	}else{
		printf("Moviendo Archivo/Directorio de: %s a: %s \n", ubicacionVieja, ubicacionNueva);
	}
}

void mostrarContenidoArchivo(char * archivo){
	if(archivo == NULL){
		printf("Falta especificar el archivo\n");
	}else{
		printf("Mostrando contenido de: %s \n", archivo);
	}
}

void crearDirectorio(char* path){
	if(path == NULL){
		printf("Falta especificar el directorio a crear\n");
	}else{
		int idDirPadre = dirGetIndex(dirname(string_duplicate(path)));
		char* nombreDir = basename(string_duplicate(path));
		if (!existeDirectorio(nombreDir, idDirPadre)) {
			crearDir(filesystem.directorios, nombreDir, idDirPadre);
			printf("El directorio se creo correctamente\n");
		}else{
			printf("El directorio ya existe\n");
		}
	}
}

void copiarFrom(char* archivoLocal, char* dirFs, char* tipo) {
	if (archivoLocal == NULL || dirFs == NULL || tipo == NULL) {
		printf("Falta especificar origen, destino o tipo del archivo\n");
	} else {

		if (existeArchivo(archivoLocal)) {

			char* nombre = basename(string_duplicate(archivoLocal));
			int dirId = dirGetIndex(dirFs);

			if(!existeArchivoEnFS(nombre, dirId)){

				if(copiarArchivoLocalAlFs(archivoLocal, tipo, dirId) < 0){
					puts("No se pudo copiar el archivo local a YAMAfs");
				}else{
					puts("El archivo local fue copiado exitosamente");
				}
			}
		}else{
			printf("el archivo no existe: %s\n", archivoLocal);
		}
	}
}

void copiarTo(char* origen, char* destino){
	if(origen == NULL || destino == NULL){
		printf("Falta especificar origen o destino del archivo\n");
	}else{
		printf("Copiando archivo: %s a: %s \n", origen, destino);
	}
}

void copiarBloqueANodo(char* archivo, char* bloque, char* nodo){
	if(archivo == NULL || bloque == NULL || nodo == NULL){
		printf("Falta especificar archivo,bloque o nodo\n");
	}else{
		printf("Copiando bloque: %s del archivo: %s al nodo: %s \n", bloque, archivo,nodo);
	}
}

void mostrarMD5(char* archivo){
	if(archivo == NULL){
		printf("Falta especificar el archivo\n");
	}else{
		printf("Mostrando MD5 de: %s \n", archivo);
		system("md5sum /home/utnso/prueba.txt");
	}
}

void mostrarArchivosDelDirectorio(char* directorio){
	if(directorio == NULL){
		printf("Falta especificar el directorio\n");
	}else{
		char* nombre = basename(string_duplicate(directorio));
		int idDirPadre = dirGetIndex(dirname(string_duplicate(directorio)));
		if(existeDirectorio(nombre, idDirPadre)){
			printf("Listando los archivos de: %s \n", directorio);
			int idDir = dirGetIndex(directorio);
			void _print_archivo_dir(t_archivo* archivo){
				if(archivo->info->directorio == idDir){
					printf("%s\n", archivo->info->nombre);
				}
			}
			list_iterate(filesystem.archivos, (void*)_print_archivo_dir);
		}else{
			puts("El directorio no existe");
		}
	}
}

void mostrarInfoArchivo(char* pathArchivo){
	if(pathArchivo == NULL){
		printf("Falta especificar el archivo\n");
	}else{
		t_archivo* archivo = NULL;
		archivo = buscarArchivoPorNombreAbsoluto(pathArchivo);
		mostrarInfoCompletaArchivo(archivo);
	}
}

/* FIN FUNCIONES CONSOLA */

int copiarArchivoLocalAlFs(char* nombre, char* tipo, int dirPadre) {

	t_list* bloques_de_datos = NULL;

	char* archivomapeado= mapearArchivo(nombre);

	bloques_de_datos = partirArchivoEnBloques(archivomapeado);

	if(bloques_de_datos == NULL){
		desmapearArchivo(archivomapeado, nombre);
		return -1;
	}

	t_archivo* archivo = malloc(sizeof *archivo);
	archivo->bloquesDeDatos = bloques_de_datos;

	t_archivo_info* info = NULL;
	string_to_upper(tipo);
	info = getInfoArchivo(nombre, tipo, dirPadre);
	info->cantBloques = list_size(bloques_de_datos);
	archivo->info = info;

	crearArchivoMetadata(archivo);

	list_add(filesystem.archivos, archivo);
	desmapearArchivo(archivomapeado, nombre);
	persistirNodos();
	return 1;
}

int cantBloquesNecesarios(char* archivo) {
	int cant = 0;
	size_t size = string_length(archivo);

	cant = size / TAMANIO_BLOQUE;

	if (size % TAMANIO_BLOQUE != 0)
		cant++;

	return cant;
}

int lenHastaEnter(char* strings){
	int i=0;
	while(strings[i]!='\n' && strings[i]!='\0')
		i++;

	return i+1;
}

t_list* partirArchivoEnBloques(char* archivo) {
	size_t size = string_length(archivo);
	printf("Tamanio del archivo : %zd bytes \n", size);
	int cantBloques = cantBloquesNecesarios(archivo);
	printf("Cantidad de bloques necesarios: %d \n", cantBloques);
	int cantBloquesTotales = cantBloques * CANT_COPIAS_BLOQUE;
	printf("Cantidad de bloques totales sumando copias: %d \n", cantBloquesTotales);
	printf("Cantidad de bloques libres del FS: %d\n", cantBloquesLibresFs());
	if (cantBloquesTotales > cantBloquesLibresFs()) {
		printf("No se puede guardar el archivo debido a que no hay la cantidad de bloques libres suficientes en YAMAfs\n");
		return NULL;
	}

	t_list* bloques = list_create();
	int bloqueNumero = 0;
	size_t bytes_leidos = 0, offset = 0;
	int len_aux;
	for (bytes_leidos = 0, offset = 0; bytes_leidos + offset < size;) {
		len_aux = lenHastaEnter(archivo + bytes_leidos + offset);
		if (bytes_leidos + len_aux < TAMANIO_BLOQUE) {
			bytes_leidos += len_aux;
		} else {
			//si supera el tamaño de bloque grabo
			t_archivo_bloque* bloqueArchivo = crearBloqueArchivo(bloqueNumero, bytes_leidos);
			bloqueNumero++;
			bloqueArchivo->nodosBloque = distribuirBloque();
			int status = guardarBloque(archivo+offset, bytes_leidos, bloqueArchivo);
			if (status < 0) {
				puts("no se pudo enviar el bloque a los nodos correspondientes");
				freeNull(bloques);
				freeNull(bloqueArchivo->nodosBloque);
				freeNull(bloqueArchivo);
				return NULL;
			}
			list_add(bloques, bloqueArchivo);
			printf("El tamaño del bloque es: %d \n", bytes_leidos);
			offset += bytes_leidos;
			bytes_leidos = 0;
		}
	}
	//me fijo si quedo algo sin grabar en el bloque
	if (bytes_leidos > 0) {
		t_archivo_bloque* bloqueArchivo = crearBloqueArchivo(bloqueNumero, bytes_leidos);
		bloqueArchivo->nodosBloque = distribuirBloque();
		int status = guardarBloque(archivo+offset, bytes_leidos, bloqueArchivo);
		if (status < 0) {
			puts("no se pudo enviar el bloque a los nodos correspondientes");
			freeNull(bloques);
			freeNull(bloqueArchivo->nodosBloque);
			freeNull(bloqueArchivo);
			return NULL;
		}
		list_add(bloques, bloqueArchivo);
		printf("El tamaño del bloque es: %d \n", bytes_leidos);
	}

	return bloques;

}

void iniciarServer(void* arg){

	multiplexar((char*)arg,(void*)procesarMensaje);

}

int procesarMensaje(int fd){
	u_int32_t header = recibirHeader(fd);
	if (header <= 0){
		return -1;
	}
	switch(header){
	case HANDSHAKE_NODO:
		if(recibirInfoNodo(fd) < 0){
			return -1;
		};
		break;
	case ALMACENAR_ARCHIVO:
		//almacenarArchivo(fd);
		break;
	}
	return 1;
}

int recibirInfoNodo(int fd){

	u_int32_t cantBloquesData = recibirInt(fd);
	char* nombre = recibirString(fd);
	u_int32_t puerto = recibirInt(fd);

	if(puerto <= 0 || nombre == NULL || cantBloquesData <= 0){
		return -1;
	}
	char* ipNodo = getIpSocket(fd);
	if(ipNodo == NULL){
		puts("Error al obtener IP del Nodo");
		return -1;
	}
	t_nodo* nodo = crearNodo(fd, nombre, ipNodo, string_itoa(puerto), cantBloquesData);
	list_add(filesystem.nodos, nodo);
	printInfoNodo(nodo);
	freeNull(nombre);
	return 1;
}

void crearFilesystem(){
	filesystem.nodos = list_create();
	filesystem.directorios = list_create();
	filesystem.archivos = list_create();
}

t_nodo* crearNodo(int fd, char* nombre, char* ipNodo, char* puerto, u_int32_t tamanioData){
	t_nodo* nodo = malloc(sizeof(t_nodo));
	nodo->info = malloc(sizeof(t_nodo_info));
	nodo->info->nombre = string_duplicate(nombre);
	strcpy(nodo->info->ip, ipNodo);
	nodo->info->puerto = string_duplicate(puerto);
	nodo->fd = fd;
	nodo->conectado = true;
	nodo->bloques = crearBitmap(tamanioData);
	nodo->cantBloques = tamanioData;
	return nodo;
}

void printInfoNodo(t_nodo* nodo){
	printf("Se creo un nodo. Esta es su info:\n");
	printf("Nombre:%s\n", nodo->info->nombre);
	printf("IP: %s\n", nodo->info->ip);
	printf("Puerto:%s\n", nodo->info->puerto);
	printf("Socket:%d\n", nodo->fd);
	if(nodo->conectado){
		printf("Estado: conectado\n");
	}else{
		printf("Estado: desconectado\n");
	}
	printf("El tamaño del bitmap del nodo es: %d\n", nodo->cantBloques);
}

t_bitmap* crearBitmap(u_int32_t tamanio) {
	t_bitmap* bitmap = malloc(sizeof(t_bitmap) * tamanio);
	int i;
	for(i=0; i < tamanio; i++){
		bitmap[i].ocupado = 0;
	}
	return bitmap;
}

void crearServer(char* puerto){
	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);

	pthread_create( &hiloServer, &tattr, (void*)iniciarServer, (void*)puerto);
	pthread_attr_destroy(&tattr);
}

int cantBloquesLibresNodo(t_nodo* nodo){
	int cant = 0;
	int i;
	for(i=0; i < nodo->cantBloques; i++){
		if(nodo->bloques[i].ocupado == 0){
			cant++;
		}
	}
	return cant;
}

int cantTotalBloquesFs(){
	int cant = 0;
	void sumarBloquesNodos(t_nodo* nodo){
		cant += nodo->cantBloques;
	}
	list_iterate(filesystem.nodos, (void*)sumarBloquesNodos);
	return cant;
}

int cantBloquesLibresFs(){
	int cant = 0;
	void sumarBloquesLibresNodos(t_nodo* nodo){
		cant += cantBloquesLibresNodo(nodo);
	}
	list_iterate(filesystem.nodos, (void*)sumarBloquesLibresNodos);
	return cant;
}

t_list* distribuirBloque(){
	t_list* bloqueCopias = list_create();
	t_archivo_nodo_bloque* anb1 = crearArchivoNodoBloque();
	t_archivo_nodo_bloque* anb2 = crearArchivoNodoBloque();
	t_nodo* nodo1 = getNodoParaGuardarBloque(filesystem.nodos);
	if(nodo1 == NULL){
		puts("No se pudo distribuir el bloque");
		destruirArchivoNodoBloque(anb1);
		destruirArchivoNodoBloque(anb2);
		return NULL;
	}
	anb1->info = nodo1->info;
	anb1->numeroBloque = getBloqueLibreNodo(nodo1);

	t_nodo* nodo2 = getNodoParaGuardarBloqueDistintoANodo(filesystem.nodos, nodo1);
	if(nodo1 == NULL){
		puts("No se pudo distribuir el bloque");
		destruirArchivoNodoBloque(anb1);
		destruirArchivoNodoBloque(anb2);
		return NULL;
	}
	anb2->info = nodo2->info;
	anb2->numeroBloque = getBloqueLibreNodo(nodo2);

	list_add(bloqueCopias, anb1);
	list_add(bloqueCopias, anb2);

	return bloqueCopias;
}

t_archivo_nodo_bloque* crearArchivoNodoBloque(){
	t_archivo_nodo_bloque* tanb = malloc(sizeof(t_archivo_nodo_bloque));
	tanb->info = malloc(sizeof(t_nodo_info));
	return tanb;
}

int getBloqueLibreNodo(t_nodo* nodo){
	int pos;
	int i = 0;
	bool encontrado = false;
	while(!encontrado && (i < nodo->cantBloques)){
		if(nodo->bloques[i].ocupado == 0){
			encontrado = true;
			pos = i;
		}else{
			i++;
		}
	}
	return pos;
}

bool ordenarPorMayorCantBloquesLibres(t_nodo* nodo1, t_nodo* nodo2) {
	return cantBloquesLibresNodo(nodo1) > cantBloquesLibresNodo(nodo2);
}

t_nodo* getNodoParaGuardarBloque(t_list* nodos){
	list_sort(nodos, (void*) ordenarPorMayorCantBloquesLibres);
	t_nodo* nodo = list_get(nodos, 0);
	return nodo;
}

t_nodo* getNodoParaGuardarBloqueDistintoANodo(t_list* nodos, t_nodo* nodo0){
	list_sort(nodos, (void*) ordenarPorMayorCantBloquesLibres);
	bool encontrado = false;
	int i = 0;
	t_nodo* nodo1 = NULL;
	while(!encontrado && (i < list_size(nodos))){
		t_nodo* aux = list_get(nodos, i);
		if(aux->info->nombre != nodo0->info->nombre){
			nodo1 = aux;
			encontrado = true;
		}else{
			i++;
		}
	}
	return nodo1;
}

int enviarBloqueANodos(t_archivo_bloque* bloqueArchivo, char* bloque){
	t_archivo_nodo_bloque* nb1 = list_get(bloqueArchivo->nodosBloque, 0);
	int fd1 = buscarNodoPorNombre(nb1->info->nombre)->fd;
	t_archivo_nodo_bloque* nb2 = list_get(bloqueArchivo->nodosBloque, 1);
	int fd2 = buscarNodoPorNombre(nb2->info->nombre)->fd;
	printf("FD nodo 1: %d\n", fd1);
	printf("FD nodo 2: %d\n", fd2);
	puts("Enviando bloque");
	enviarHeader(fd1, SETBLOQUE);
	puts("Header enviado!");
	enviarInt(fd1, nb1->numeroBloque);
	puts("Numero de bloque enviado!");
	printf("tamaño del bloque:%d\n", string_length(bloque));
	if (enviarString(fd1, bloque) <= 0){
		perror("Error al mandar el bloque al nodo\n");
		freeNull(nb1);
		freeNull(nb2);
		freeNull(bloque);
		return -1;
	}else{
		marcarBloqueComoUsado(nb1->info->nombre, nb1->numeroBloque);
		puts("Bloque 1 enviado al primer nodo!");
	}
	enviarHeader(fd2, SETBLOQUE);
	puts("Header enviado!");
	enviarInt(fd2, nb2->numeroBloque);
	puts("Numero de bloque enviado!");
	if (enviarString(fd2, bloque) <= 0){
		perror("Error al mandar el bloque al nodo\n");
		freeNull(nb1);
		freeNull(nb2);
		freeNull(bloque);
		return -1;
	}else{
		marcarBloqueComoUsado(nb2->info->nombre, nb2->numeroBloque);
		puts("Bloque 2 enviado al segundo nodo!");
	}
	freeNull(bloque);
	return 1;
}

t_nodo* buscarNodoPorNombre(char* nombre){
	bool _buscar_nodo_por_nombre(t_nodo* nodo) {
		return nodo->info->nombre == nombre;
	}
	return list_find(filesystem.nodos, (void*) _buscar_nodo_por_nombre);
}

void marcarBloqueComoUsado(char* nombre, int numeroBloque){
	t_nodo* nodo = buscarNodoPorNombre(nombre);
	nodo->bloques[numeroBloque].ocupado = 1;
}

void marcarBloqueComoLibre(char* nombre, int numeroBloque){
	t_nodo* nodo = buscarNodoPorNombre(nombre);
	nodo->bloques[numeroBloque].ocupado = 0;
}

int guardarBloque(char* data, size_t tamanio, t_archivo_bloque* ab){
	char* bloque = malloc(tamanio + 1);
	memcpy(bloque, data, tamanio);
	bloque[tamanio] = '\0';
	return enviarBloqueANodos(ab, bloque);
}

void persistirNodos(){
	limpiarArchivo(FILE_NODOS);
	FILE* file = txt_open_for_append(FILE_NODOS);
	char* tamanio = string_new();
	string_append_with_format(&tamanio, "TAMANIO=%s", cantTotalBloquesFs());
	txt_write_in_file(file, tamanio);
	char* libre = string_new();
	string_append_with_format(&libre, "LIBRE=%d", cantBloquesLibresFs());
	txt_write_in_file(file, libre);
	char* nodos = string_new();
	string_append(&nodos, "NODOS=[");
	int i;
	for (i = 0; i < list_size(filesystem.nodos); i++){
		t_nodo* nodo = list_get(filesystem.nodos, i);
		string_append_with_format(&nodos, "%s,", nodo->info->nombre);
	}
	char* nodosAux = string_new();
	string_append(&nodosAux, string_substring_until(nodos, string_length(nodos) - 1));
	string_append(&nodosAux, "]");
	txt_write_in_file(file, nodosAux);
	for (i = 0; i < list_size(filesystem.nodos); i++){
		char* nodoTotal = string_new();
		t_nodo* nodo = list_get(filesystem.nodos, i);
		string_append_with_format(&nodoTotal, "%sTotal=%d", nodo->info->nombre, nodo->cantBloques);
		txt_write_in_file(file, nodoTotal);
		char* nodoLibre = string_new();
		string_append_with_format(&nodoLibre, "%sLibre=%d", nodo->info->nombre, cantBloquesLibresNodo(nodo));
		txt_write_in_file(file, nodoLibre);
		freeNull(nodoTotal);
		freeNull(nodoLibre);
	}
	freeNull(tamanio);
	freeNull(libre);
	freeNull(nodos);
	freeNull(nodosAux);
	txt_close_file(file);
	persistirBitmaps();
}

void persistirBitmaps(){
	int i, j;
	for (i = 0; i < list_size(filesystem.nodos); i++){
		t_nodo* nodo = list_get(filesystem.nodos, i);
		char* path = string_new();
		string_append(&path, DIRECTORIO_BITMAPS);
		string_append_with_format(&path, "%s.dat", nodo->info->nombre);
		limpiarArchivo(path);
		FILE* file = txt_open_for_append(path);
		char* bitmap = string_new();
		for (j = 0; i < nodo->cantBloques; i++){
			string_append_with_format(&bitmap, "%s", string_itoa(nodo->bloques[j].ocupado));
		}
		txt_write_in_file(file, bitmap);
		txt_close_file(file);
		freeNull(path);
		freeNull(bitmap);
	}
}

/* INICIO FUNCIONES DE ARCHIVOS */

bool existeArchivoEnFS(char* nombre, int dir_id) {

	return buscarArchivoPorNombre(filesystem.archivos, nombre, dir_id) != NULL;
}

t_archivo_info* getInfoArchivo(char* nombre, char* tipo, int dirPadre) {
	t_archivo_info* info = malloc(sizeof *info);
	info->disponible = true;
	info->tipo = tipo;
	info->nombre = basename(string_duplicate(nombre));
	info->tamanio = tamanioArchivo(nombre);
	info->directorio = dirPadre;

	return info;
}

void destruirArchivoNodoBloque(t_archivo_nodo_bloque* anb1){
	freeNull(anb1->info);
	freeNull(anb1);
}

t_archivo_bloque* crearBloqueArchivo(int numero, int tamanio){
	t_archivo_bloque* bloqueArchivo = malloc(sizeof(t_archivo_bloque));
	bloqueArchivo->numeroBloque = numero;
	bloqueArchivo->tamanio = tamanio;
	return bloqueArchivo;
}

t_archivo_bloque* buscarBloqueArchivo(t_archivo* archivo, int numeroBloque){
	t_archivo_bloque* bloque = NULL;

	bool _buscar_bloque(t_archivo_bloque* bloque){
		return bloque->numeroBloque == numeroBloque;
	}

	bloque = list_find(archivo->bloquesDeDatos, (void*)_buscar_bloque);

	return bloque;
}

void mostrarInfoCompletaArchivo(t_archivo* archivo){
	printInfoArchivo(archivo->info);
	mostrarBloquesArchivo(archivo->bloquesDeDatos);
}

void mostrarBloquesArchivo(t_list* bloques_de_datos){

	void mostrar_bloque_datos(t_archivo_bloque* bloque_datos){
		printf("Bloque Nro: %d\n", bloque_datos->numeroBloque);

		int i=1;
		void mostrar_nodo_bloque(t_archivo_nodo_bloque* nodo_bloque){
			printf("Copia %d: nodo: %s, bloque nro: %d\n", i, nodo_bloque->info->nombre, nodo_bloque->numeroBloque);
			i++;
		}

		list_iterate(bloque_datos->nodosBloque, (void*)mostrar_nodo_bloque);
	}

	list_iterate(bloques_de_datos, (void*)mostrar_bloque_datos);
}

void printInfoArchivo(t_archivo_info* info){
	printf("Info del archivo %s\n", info->nombre);
	printf("Tamanio : %zd b, %.2f kb, %.2f mb\n", info->tamanio, bytesToKilobytes(info->tamanio), bytesToMegabytes(info->tamanio));

	printf("Directorio padre: %d\n", info->directorio);
	printf("Disponible: %d\n", info->disponible);
	printf("Cantidad de bloques: %d\n", info->cantBloques);
}

t_archivo* buscarArchivoPorNombre(t_list* archivos, char* nombre, int dirId) {
	t_archivo* archivo = NULL;
	bool _buscar_archivo_por_nombre(t_archivo* archivo) {
		return string_equals_ignore_case(archivo->info->nombre, nombre)
				&& archivo->info->directorio == dirId;
	}
	archivo = list_find(archivos, (void*) _buscar_archivo_por_nombre);
	return archivo;
}

t_archivo* buscarArchivoPorNombreAbsoluto(char* path_abs){
	char* dir = malloc(strlen(path_abs)+1);
	char* name = malloc(strlen(path_abs)+1);

	strcpy(dir, path_abs);
	strcpy(name, path_abs);

	dir = dirname(dir);

	name = basename(name);

	int dir_id = dirGetIndex(dir);

	freeNull(dir);

	t_archivo* archivo = buscarArchivoPorNombre(filesystem.archivos, name, dir_id);

	return archivo;
}

void renombrarArchivo(char* nombreViejo, char* nombreNuevo){
	t_archivo* archivo = buscarArchivoPorNombreAbsoluto(nombreViejo);
	char* pathMetadataOld = getPathMetadataArchivo(archivo);
	strcpy(archivo->info->nombre, nombreNuevo);
	char* pathMetadataNew = getPathMetadataArchivo(archivo);
	int rs = rename(pathMetadataOld, pathMetadataNew);
	if(rs == 0){
		puts("Archivo renombrado exitosamente");
	}else{
		puts("No se pudo renombrar el archivo");
	}
	freeNull(pathMetadataOld);
	freeNull(pathMetadataNew);
}

void crearArchivoMetadata(t_archivo* archivo){
	char* path = string_new();
	string_append(&path, DIRECTORIO_ARCHIVOS);
	string_append(&path, string_itoa(archivo->info->directorio));
	DIR* dir = opendir(path);
	if (dir)
	{
		/* Directorio existe */

		char** nombre = string_split(archivo->info->nombre, ".");
		string_append_with_format(&path, "/%s.csv", nombre[0]);
		persistirArchivo(archivo, path);
		destroySplit(nombre);
		closedir(dir);
	}
	else if (ENOENT == errno)
	{
		/* Directorio no existe */

		mkdir(path, 0777);
		char** nombre = string_split(archivo->info->nombre, ".");
		string_append_with_format(&path, "/%s.csv", nombre[0]);
		persistirArchivo(archivo, path);
		destroySplit(nombre);
	}

	freeNull(path);
}

void persistirArchivo(t_archivo* archivo, char* path){
	limpiarArchivo(path);
	FILE* file = txt_open_for_append(path);
	char* tamanio = string_new();
	string_append_with_format(&tamanio, "TAMANIO=%s", string_itoa(archivo->info->tamanio));
	txt_write_in_file(file, tamanio);
	char* tipo = string_new();
	string_append_with_format(&tipo, "TIPO=%s", archivo->info->tipo);
	txt_write_in_file(file, tipo);
	int i, j;
	for (i = 0; i < archivo->info->cantBloques; i++){
		t_archivo_bloque* ab = list_get(archivo->bloquesDeDatos, i);
		for (j = 0; i < list_size(ab->nodosBloque); i++){
			char* bloque = string_new();
			t_archivo_nodo_bloque* anb = list_get(ab->nodosBloque, j);
			string_append_with_format(&bloque, "BLOQUE%dCOPIA%d=[%s,%d]", i, j, anb->info->nombre, anb->numeroBloque);
			txt_write_in_file(file, bloque);
			freeNull(bloque);
		}
		char* tamanioBloque = string_new();
		string_append_with_format(&tamanio, "BLOQUE%dBYTES=%d", i, ab->tamanio);
		txt_write_in_file(file, tamanioBloque);
		freeNull(tamanioBloque);
	}
	txt_close_file(file);
	freeNull(tamanio);
	freeNull(tipo);
}

char* getPathMetadataArchivo(t_archivo* archivo){
	char* path = string_new();
	string_append(&path, DIRECTORIO_ARCHIVOS);
	string_append(&path, string_itoa(archivo->info->directorio));
	char** nombre = string_split(archivo->info->nombre, ".");
	string_append_with_format(&path, "/%s.csv", nombre[0]);
	destroySplit(nombre);
	return path;
}

void borrarArchivoFs(char* nombre, int dirId){
	t_archivo* archivo = buscarArchivoPorNombre(filesystem.archivos, nombre, dirId);
	void _liberar_bloque(t_archivo_bloque* ab){
		void _liberar_copia(t_archivo_nodo_bloque* anb){
			marcarBloqueComoLibre(anb->info->nombre, anb->numeroBloque);
		}
		list_iterate(ab->nodosBloque, (void*)_liberar_copia);
	}
	list_iterate(archivo->bloquesDeDatos, (void*)_liberar_bloque);
	bool _es_archivo_buscado(t_archivo* arch){
		return arch->info->nombre == nombre && arch->info->directorio == dirId;
	}
	list_remove_and_destroy_by_condition(filesystem.archivos, (void*)_es_archivo_buscado, (void*)destroyArchivo);
	char* pathMetadata = getPathMetadataArchivo(archivo);
	remove(pathMetadata);
	freeNull(pathMetadata);
}

void destroyArchivo(t_archivo* archivo){
	freeNull(archivo->info);
	list_destroy_and_destroy_elements(archivo->bloquesDeDatos, (void*)destroyBloqueDeDatos);
	freeNull(archivo);
}

void destroyBloqueDeDatos(t_archivo_bloque* bloque_de_datos){
	list_destroy_and_destroy_elements(bloque_de_datos->nodosBloque,(void*)destruirArchivoNodoBloque);
	freeNull(bloque_de_datos)
}

/* FIN FUNCIONES DE ARCHIVOS */

/* INICIO FUNCIONES DE DIRECTORIOS */

void destruirDirectorio(t_directorio* directorio){
	freeNull(directorio);
}

int eliminarDirectorioPorId(t_list* directorios, int id){

	char* map = mapearArchivo(FILE_DIRECTORIO);

	t_directorio* dir = malloc(sizeof*dir);
	memcpy(dir, map + ((id-1)*sizeof(t_directorio)), sizeof(t_directorio));

	dir->index = 0;
	memset(dir->nombre, 0, sizeof(dir->nombre));
	dir->padre = -1;

	memcpy(map + ((id-1)*sizeof(t_directorio)), dir, sizeof(t_directorio));

	freeNull(dir);
	desmapearArchivo(map, FILE_DIRECTORIO);

	bool _buscar_directorio_por_id(t_directorio* dir){
		return dir->index == id;
	}
	list_remove_and_destroy_by_condition(directorios, (void*)_buscar_directorio_por_id, (void*)destruirDirectorio);


	return 0;
}

int renombrarDirectorio(t_list* directorios, int id, char* nuevoNombre){

	char* map = mapearArchivo(FILE_DIRECTORIO);

	t_directorio* dir = malloc(sizeof *dir);

	memcpy(dir, map + ((id - 1) * sizeof(t_directorio)), sizeof(t_directorio));

	strcpy(dir->nombre, nuevoNombre);

	memcpy(map + ((id - 1) * sizeof(t_directorio)), dir, sizeof(t_directorio));

	freeNull(dir);
	desmapearArchivo(map, FILE_DIRECTORIO);

	dir = buscarDirectorioPorId(directorios, id);
	strcpy(dir->nombre, nuevoNombre);
	puts("Directorio renombrado exitosamente");

	return 0;
}

t_directorio* buscarDirectorioPorNombre(t_list* directorios, char* nombre, int padre){
	bool _dir_buscar_por_nombre(t_directorio* dir){
		return string_equals_ignore_case(dir->nombre, nombre) && dir->padre == padre;
	}
	return list_find(directorios, (void*)_dir_buscar_por_nombre);
}

t_directorio* buscarDirectorioPorId(t_list* directorios, int id){
	bool _dir_buscar_por_id(t_directorio* dir){
		return dir->index == id;
	}
	return list_find(directorios, (void*)_dir_buscar_por_id);
}

int obtenerUltimoIndexDirectorios() {

	char* map = mapearArchivo(FILE_DIRECTORIO);
	int index_new = -1;

	t_directorio* dir;
	dir = malloc(sizeof *dir);
	int i = 0;
	for (i = 0; i < DIR_CANT_MAX; i++) {

		dir = memcpy(dir, map + (i * sizeof(t_directorio)),	sizeof(t_directorio));
		if (dir->index == 0) {
			index_new = i + 1;
			break;
		}
	}
	freeNull(dir);
	desmapearArchivo(map, FILE_DIRECTORIO);
	return index_new;
}

void printDirectorio(t_directorio* directorio) {
	printf("Index: %d, Padre: %d, Nombre: %s\n", directorio->index, directorio->padre, directorio->nombre);
}

int crearDir(t_list* directorios, char* nombre, int padre) {
	int index = obtenerUltimoIndexDirectorios();
	if (index == -1) {
		printf("No se pueden crear mas de %d directorios\n", DIR_CANT_MAX);
		return -1;
	}

	t_directorio* dir = malloc(sizeof *dir);
	dir->index = index;
	strcpy(dir->nombre, nombre);
	dir->padre = padre;

	char* map = mapearArchivo(FILE_DIRECTORIO);

	memcpy(map + ((dir->index-1)*sizeof(t_directorio)), dir, sizeof(t_directorio));

	list_add(directorios, (void*)dir);

	printf("se creo el directorio %s con padre %d e indice %d\n", dir->nombre,dir->padre, dir->index);

	return 0;
}

void formatearDirectorios() {

	crearArchivo(FILE_DIRECTORIO, DIR_CANT_MAX * (sizeof(t_directorio)));

	char* map = mapearArchivo(FILE_DIRECTORIO);
	t_directorio* dir = NULL;
	dir = malloc(sizeof(t_directorio));
	dir->index = 0;
	memset(dir->nombre, 0 , 255);
	dir->padre = -1;

	int i;
	for (i = 0; i < DIR_CANT_MAX; i++) {
		memcpy(map + i * sizeof(t_directorio), dir, sizeof(t_directorio));
	}

	freeNull(dir);

	desmapearArchivo(map, FILE_DIRECTORIO);

}

bool existeDirectorio(char* nombre, int padre) {
	return buscarDirectorioPorNombre(filesystem.directorios, nombre, padre) != NULL;
}

bool tieneArchivosDir(int dirId){
	bool _archivo_esta_en_dir(t_archivo* archivo){
		return archivo->info->directorio == dirId;
	}
	return list_any_satisfy(filesystem.archivos, (void*)_archivo_esta_en_dir);
}

int dirGetIndex(char* path){
	//si solo pasa el / le devuelvo directamente el raiz
	if(strcmp(path, "/") == 0){
		return 0;
	}
	int padre;
	//verifico si esta usando / en el primer char, si es asi uso a padre = 0;
	if(path[0] == '/')
		padre = 0;

	int rs;
	char** nombres = string_split(path, "/");
	int i=0;
	t_directorio* dir = NULL;
	while(nombres[i]!=NULL){

		bool _buscar_en_dirs(t_directorio* dir){
			return string_equals_ignore_case(dir->nombre, nombres[i])  && dir->padre == padre;
		}

		dir = list_find(filesystem.directorios, (void*)_buscar_en_dirs);
		if(dir==NULL){
			rs = -1;
			break;
		}else{
			//asigno el nuevo padre
			padre = dir->index;
			rs = dir->index;
		}

		i++;
	}
	//limpio el resultado del split

	destroySplit(nombres);

	return rs;
}
/* FIN FUNCIONES DE DIRECTORIOS */
