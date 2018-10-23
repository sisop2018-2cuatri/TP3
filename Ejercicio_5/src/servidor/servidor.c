#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <protocol.h>
#include <string.h>

void err_sys(char* err_msg){
	fprintf(stderr, "%s\n", err_msg);
}

int main(){
	char* home = getenv("HOME");
	s_config configuracion;
	load_config(strcat(home,"/.config/adlms/Ejercicio5/conf.conf"),&configuracion);
	printf("Estas son las configuraciones:\nHOME:%s\nNombre Mem: %s\nTamanio Mem: %d",home,configuracion.name,configuracion.size);

	exit(0);
}
