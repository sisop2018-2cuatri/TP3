
/*
 *EJERCICIO 5
 *Git: https://github.com/sisop2018-2cuatri/TP3/Ejercicio_5
 *Programa: servidor.c
 *Descripcion: Genera memoria compartida y administra la db segun los mensajes que se pasen por esta
 *Autor: maximiliano-rdl
 *Fecha: 24/11/2018
 **/

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <protocol.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <servidor.h>
#include <defines.h>

int main(){
	char* home_dir = getenv("HOME");
	s_config configuracion;
	int fd;
	char* local_path="../conf/config.conf";
	
	if(load_config(local_path,&configuracion)== READ_FAILURE){
		debug_sys("No hay archivo local, configuracion por defecto");
		configuracion.modo_ejecucion=1;
		configuracion.cantidad_clientes_maxima=10;
		configuracion.mostrar_ayuda=1;
	}	
	mostrar_config(&configuracion);

	fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
	if(fd<0){
		perror("shm_open()");
		return EXIT_FAILURE;
	}

	ftruncate(fd, SIZE);

	int *data = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("sender mapped address: %p\n",data);

	for(int i = 0; i < 4; ++i){
		data[i]=i;
	}
	munmap(data, SIZE);

	close(fd);

	return EXIT_SUCCESS;
}

void err_sys(char* err_msg){
	fprintf(stderr, "%s\n", err_msg);
}

void debug_sys(char* dbg_msg){
	printf("DEBUG: %s\n", dbg_msg);
}

