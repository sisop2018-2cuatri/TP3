#include <stdio.h>
#include <stdlib.h>
#include <protocol.h>
#include <string.h>
#include <string_utils.h>
#include <defines.h>

/* Lectura del archivo de configuracion*/
int load_config(char* conf_path, s_config* conf){
	
	const char* opciones[]={
		"MODO_EJECUCION",
		"MOSTRAR_AYUDA",
		"CANTIDAD_CLIENTES_MAXIMA"
	};

	FILE* fp= fopen(conf_path,"r");
	char line[80];
		
	if(fp == NULL){
		return READ_FAILURE;
	}

	while((fgets(line,80,fp)) != NULL){
		if(prefix(opciones[0],line)){
			conf->modo_ejecucion=atoi(line+15);
		}
		if(prefix(opciones[1],line)){
			conf->mostrar_ayuda=atoi(line+14);
		}
		if(prefix(opciones[2],line)){
			conf->cantidad_clientes_maxima=atoi(line+24+1);
		}
	}
	fclose(fp);
}

void mostrar_config(s_config *conf){
	printf("CANTIDAD MAXIMA CLIENTES: %d\n", conf->cantidad_clientes_maxima);
	printf("MOSTRAR AYUDA: %d\n", conf->mostrar_ayuda);
	printf("MODO DE EJECUCION: %d\n", conf->modo_ejecucion);
}
