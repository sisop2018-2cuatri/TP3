#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

typedef struct config{
	int modo_ejecucion;
	int cantidad_clientes_maxima;
	int mostrar_ayuda;
} s_config;

int load_config(char*, s_config*);
void mostrar_config(s_config*);
int crear_arch_conf(char*);
int cargar_config(s_config*);

#endif
