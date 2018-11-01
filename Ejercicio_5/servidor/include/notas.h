#ifndef NOTAS_H
#define NOTAS_H
#define ASCENDENTE 1
#define DESCENDENTE 0
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

enum TIPOS_EVALUACION
{
    PARCIAL_UNO,
    PARCIAL_DOS,
    RECUPERATORIO
};

typedef struct registro
{
    int dni;
    char materia[30];
    enum TIPOS_EVALUACION evaluacion;
    float nota;
} t_registro;

typedef struct _nodo
{
    t_registro registro;
    struct _nodo *siguiente;
    struct _nodo *anterior;
} tipoNodo;

typedef tipoNodo *pNodo;
typedef tipoNodo *Lista;

int insertar(Lista*, t_registro);
void borrar(Lista*, t_registro);
int cmp_notas(t_registro, t_registro);
int cmp_materias(char*, char*);
void init_nota(t_registro*, int, char*, enum TIPOS_EVALUACION , float);
void vaciar_lista(Lista*);
void mostrar_registro(t_registro*);

/*
    Abre el archivo de notas
    ------------------------
    No se debe llamar dentro de un thread
    Se leen las notas desde el archivo correspondiente, si no existe lo crea.
    Si el archivo ya existe y tiene notas previamente cargadas:
        - se ordenan los registros leidos (para mantenerlo pseudo ordenado).
        - se eliminan los registros repetidos.
    Retorna:
        0: si no se pudo leer ni crear el archivo de notas
        1: si se pudieron cargar las notas
*/
int inicializar_notas(char *path_notas);

/*
    Agrega una nueva nota al archivo
    --------------------------------
    No se debe llamar antes de inicializar_notas
    Inserta al final del archivo un nuevo registro.
    Para ganar velocidad las inserciones son desordenadas.
    Parametros:
        dni: 
            número positivo mayor a cero que identifica cada alumno
        materia: 
            cadena de mil caracteres ó menos de largo, 
            no vacía y que no puede contener caracteres coma (,).
            cuyo valor repredenta la materia rendida por el alumno
        evaluacion: 
            un valor entre los posibles de TIPOS_EVALUACION,
            el cuál se usa para saber que evaluación fue rendida
        nota:
            número entre uno y diez (ambos inclusive), cuyo valor
            representa la nota del alumno en la evaluación en la materia
    Retorna:
        Todos los estados de retorno mayores que (1) 
        son reservados para casos de error
        1: si se pudo agregar la nota
        2: dni debe ser un número positivo mayor a cero
        3: materia vacía
        4: materia demasiado larga, mayor a 1000 caracteres
        5: materia no puede contener caracter coma (,)
        6: tipo de evaluación incorrecto
        7: valores de nota fuera de rango (menor a 1 ó mayor que 10)
        8: nota ya cargada previamente
        9: error al guardar en archivo
*/
int cargar_nota(int dni, char *materia, enum TIPOS_EVALUACION evaluacion, float nota);

/*
    Obtener el promedio del alumno en una materia
    ---------------------------------------------
    No se debe llamar antes de inicializar_notas
    Algoritmo de cálculo implementado
        si el alumno en la materia tiene:
            0 notas: 
                retorna cero
            1 nota: 
                retorna cero
            2 notas (parcial uno y dos, ó cualquier parcial y recuperatorio):
                retorna la sumatoria de ambas entre dos
            3 notas: 
                retorna la sumatoria de las dos mayores entre dos
    Parámetros:
        dni: del alumno del cual se consulta el promedio
        materia: nombre de la materia de la cual corresponde el promedio
    Retorna:
        mayor a 0: promedio del alumno en la materia
        0: la materia no se concidera rendida por el alumno
*/
float get_promedio(int dni, char *materia);

/*  
    Obtener promedio general del alumno
    -----------------------------------
    No se debe llamar antes de inicializar_notas
    Algoritmo de cálculo:
        se obtiene la sumatoria de promedios entre todas las materias
        rendidas y se divide entre la cantidad de materias rendidas.
    Parámetros:
        dni: del alumno del cual se consulta el promedio general
    Retorna:
        mayor a 0: promedio general del alumno
        0: sin materias rendidas por el alumno
*/
float get_promedio_general(int dni);

/*
    Mostrar listado de notas cargadas
    ---------------------------------
    No se debe llamar antes de inicializar_notas
    Se muestra en pantalla el listado de notas cargadas en memoria
*/
void mostrar_notas(int orden);

/*
    libera la memoria ocupada por la lista de notas
*/
void liberar_memoria(void);

#endif