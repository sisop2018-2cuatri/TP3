#include <config_loader.h>

#define SIZE 100000
#define NAME "shmnotas"

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

void finalizar_conexion();