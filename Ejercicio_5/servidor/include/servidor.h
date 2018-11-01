#ifndef SERVIDOR_H
#define SERVIDOR_H
#include <notas.h>
#define TAM sizeof(t_mensaje)
#define NAME "shmnotas" //nombre de la shared memory
#define DB_PATH "../db/db.dat" //localizacion de la database
#define CODIGO_INVALIDO -12 //codigo invalido recibido
#define EXITO 1
#define CARGAR 2 //defino codigo de t_mensaje para carga de nota de alumno
#define PROMAT 3 //defino codigo de t_mensaje para promedio de materia de alumno
#define PROGEN 4 //defino codigo de t_mensaje para promedio general de alumno



typedef struct s_mensaje{
	int codigo;
	long int dni;
    enum TIPOS_EVALUACION evaluacion;
	char materia[30];
	float nota;
}t_mensaje;

int main();
int inicializar_servidor();
void cerrar_servidor();
void mostrar_ayuda();
int abrir_archivo(FILE**,const char* ,const char*); //puntero a file, path, modo
void cerrar_archivo(FILE**);
int procesar_solicitud(t_mensaje*);
float prom_mat(int, char*);
float prom_gen(int);
void print_error(int);
//int cargar_nota(t_mensaje*);

#endif
