#include <stdio.h>
#include <stdlib.h>
#include <config_loader.h>
#include <string.h>
#include <string_utils.h>
#include <defines.h>
#include <print_utils.h>

/* Lectura del archivo de configuracion*/
int load_config(char* conf_path, s_config* conf){
	
	int contador_opciones = 3;
	const char* opciones[]={
		"MODO_EJECUCION",
		"MOSTRAR_AYUDA",
		"MATERIA"
	};

	FILE* fp= fopen(conf_path,"r");
	char line[80];
		
	if(fp == NULL){
		return READ_FAILURE;
	}

	while((fgets(line,80,fp)) != NULL){
		if(prefix(opciones[0],line)){
			conf->modo_ejecucion=atoi(line+15);
			contador_opciones--;
		}
		if(prefix(opciones[1],line)){
			conf->mostrar_ayuda=atoi(line+14);
			contador_opciones--;
		}
		if(prefix(opciones[2],line)){
			strncpy(conf->materia,line+8,30);
			remove_trailing_spaces(conf->materia);
			contador_opciones--;
		}
	}
	fclose(fp);

	if(contador_opciones!=0)
		return INCOMPLETE_OPTION_FILE;
	else
		return EXITO;
}

void mostrar_config(s_config *conf){
	printf("MATERIA: %s\n", conf->materia);
	printf("MOSTRAR AYUDA: %s\n", conf->mostrar_ayuda == 1? "SI": "NO");
	printf("MODO DE EJECUCION: %s\n", conf->modo_ejecucion? "DEBUG":"NORMAL");
}

int crear_arch_conf(char* local_path){
    FILE * fp = NULL;
    const char text[]=
        "//Archivo de configuracion\n//Todo lo que este con // sera ignorado, toda linea vacia sera ignorada\n//No borrar este archivo, es fundamental para el funcionamiento del servidor y clientes\n\n//DEBUG=1 RELEASE=0\nMODO_EJECUCION=1\n//MOSTRAR=1 NO_MOSTRAR=0\nMOSTRAR_AYUDA=1\n//MATERIA DEL PROFESOR QUE USE ESTE PROGRAMA\nMATERIA=Analisis de software";

    fp = fopen(local_path, "w");
    fprintf(fp,"%s", text);
    fclose(fp);
    return 1;
}

int cargar_config(s_config* configuracion){
    char* local_path="../conf/config.conf";
    int carga; //resultado de la carga

    carga = load_config(local_path,configuracion);

    if(carga != EXITO){
            if(carga == READ_FAILURE){
                printf("No hay archivo de configuracion, generar uno nuevo?");
            }
            if(carga == INCOMPLETE_OPTION_FILE){
                printf("Archivo de configuracion incompleto. generar uno nuevo?");
            }
            
            if(leer_entradaYN()==1){
                crear_arch_conf(local_path);
            }else{
            	printf("Usando valores por defecto\n");
            }
            configuracion->modo_ejecucion=1;
            strcpy(configuracion->materia,"Sistemas Operativos");
            configuracion->mostrar_ayuda=1;
    }
    return EXITO;
}