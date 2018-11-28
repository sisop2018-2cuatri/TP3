#ifndef CLIENTE_H
#define CLIENTE_H
#include <config_loader.h>

#define TAM sizeof(t_mensaje)
#define NAME "shmnotas" //nombre de la shared memory
#define EXITO 1
#define CARGAR 2 //defino codigo de t_mensaje para carga de nota de alumno
#define PROMAT 3 //defino codigo de t_mensaje para promedio de materia de alumno
#define PROGEN 4 //defino codigo de t_mensaje para promedio general de alumno

#define DNI_NEGATIVO 2
#define MATERIA_VACIA 3
#define MATERIA_LARGA 4
#define MATERIA_COMA 5
#define TIPO_EV_INV 6
#define NOTA_INVALIDA 7
#define EXITO 1
#define FALLA_GUARDADO 9
#define REPETIDO 8
#define FALLA 0
#define NOTAS_INSUF 10

enum TIPOS_EVALUACION
{
    PARCIAL_UNO,
    PARCIAL_DOS,
    RECUPERATORIO
};

typedef struct s_mensaje{
    int codigo;
    long int dni;
    enum TIPOS_EVALUACION evaluacion;
    char materia[30];
    float nota;
}t_mensaje;

/*
    Mostrar ayuda en pantalla
*/
void mostrar_ayuda(void);

/*
    Muestra el menú de opciones
*/
void mostrar_menu(void);

//  ====================================
//          FUNCIONES DEL MENÚ
//  ====================================

/*
    cargar nueva nota para un alumno
    // TODO: documentar apropiadamente
*/
void cargar_nueva_nota(void);

/*
    Obtener promedio del alumno en la materia
    // TODO: documentar apropiadamente
*/
void obtener_promedio_materia(void);

/*
    Obtener promedio general del alumno
    -----------------------------------
    Servidor retorna (char[1024]):
        -1: ocurrió un error obteniendo el promedio general
        0: el alumno no rindió ninguna materia
        mayor a cero: promedio general del alumno
*/
void obtener_promedio_general(void);

/*
    Cierra la conexión actual y 
    solicita al usuario nuevos parámetros de conexión
*/

/*
    Carga de configuraciones en directorio local.
    param: s_config*
        estructura de configuracion del programa
*/
int cargar_config(s_config*);

int inicializar_conexion();

void finalizar_conexion();

void pausa();

void print_error(int);

void print_debug(char*);

int peticion_servidor();

void get_tiempo(int);

#endif