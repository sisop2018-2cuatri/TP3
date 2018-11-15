/*
* print_utils.c
*/
#include <print_utils.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void err_sys(char* err_msg){
	fprintf(stderr, "%s\n", err_msg);
}

void debug_sys(char* dbg_msg){
	printf("DEBUG: %s\n", dbg_msg);
}

int leer_entradaYN(){
    char opcion;
    
    do{
        printf("\nY(si) o N(No)\n");
        scanf("%c",&opcion);
        opcion = tolower(opcion);
    }while(opcion != 'y' && opcion != 'n');
    
    return opcion == 'y'? 1 : 0;
}