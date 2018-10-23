#include <stdio.h>
#include <stdlib.h>
#include <protocol.h>
#include <string.h>
#include <string_utils.h>
#include <defines.h>

/* Lectura del archivo de configuracion*/
int load_config(char* conf_path, s_config* conf){
	//s_config configs;
	FILE* fp= fopen(conf_path,"r");
	char line[80];
		
	if(fp == NULL){
		return READ_FAILURE;
	}

	while((fgets(line,80,fp)) != NULL){
		if(prefix("SHM_SIZE",line)){
			conf->size=atoi(line+9);
		}
		if(prefix("NAME",line)){
			strcpy(conf->name,line+5);
		}
	}
	fclose(fp);
}
