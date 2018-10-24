
/*
 *EJERCICIO 5
 *Git: https://github.com/sisop2018-2cuatri/TP3/Ejercicio_5
 *Programa: servidor.c
 *Descripcion: Genera memoria compartida y administra la db segun los mensajes que se pasen por esta
 *Autor: maximiliano-rdl
 *Fecha: 24/11/2018
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <protocol.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


void err_sys(char* err_msg){
	fprintf(stderr, "%s\n", err_msg);
}

int main(){
	char* home = getenv("HOME");
	s_config configuracion;
	int fd;
	load_config(strcat(home,"/.config/adlms/Ejercicio5/conf.conf"),&configuracion);
	printf("Estas son las configuraciones:\nHOME:%s\nNombre Mem: %s\nTamanio Mem: %d\n",home,configuracion.name,configuracion.size);
	
	fd = shm_open(configuracion.name, O_CREAT | O_EXCL | O_RDWR, 0600);
	if(fd<0){
		perror("shm_open()");
		return EXIT_FAILURE;
	}

	ftruncate(fd, configuracion.size);

	int *data = (int *)mmap(0, configuracion.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("sender mapped address: %p\n",data);

	for(int i = 0; i < 4; ++i){
		data[i]=i;
	}
	munmap(data, configuracion.size);

	close(fd);

	return EXIT_SUCCESS;
}
