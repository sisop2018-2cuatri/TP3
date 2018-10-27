#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../headers/notas.h"

typedef struct registro
{
    int dni;
    char materia[1000];
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

// listado de registros en memoria
Lista l_notas = NULL;
// path del archivo con las notas
char *G_PATH_NOTAS;

//  ========================================================
//      funciones privadas:
//      - manejo de notas en memoria
//      - manejo de archivo
//      - auxiliares
//  ========================================================

int insertar(Lista *l, t_registro registro);
void borrar(Lista *l, t_registro registro);
int cmp_notas(t_registro a, t_registro b);
int cmp_materias(char *m1, char *m2);
void init_nota(t_registro *n_registro, int dni, char *mat, enum TIPOS_EVALUACION ev, float nota);
void vaciar_lista(Lista *);
void mostrar_registro(t_registro *n_registro);

/*
    Mostrar un nodo registro en consola
*/
void mostrar_registro(t_registro *n_registro)
{
    enum TIPOS_EVALUACION tipo = n_registro->evaluacion;

    printf("[%d, %s, %s, %.2f]\n",
           n_registro->dni,
           n_registro->materia,
           (tipo == 0) ? "PARCIAL UNO" : ((tipo == 1) ? "PARCIAL DOS" : "RECUPERATORIO"),
           n_registro->nota);
}

/*
    Inicializar valores de un registro
*/
void init_nota(t_registro *n_registro, int dni, char *mat, enum TIPOS_EVALUACION ev, float nota)
{
    n_registro->dni = dni;
    strcpy(n_registro->materia, mat);
    n_registro->evaluacion = ev;
    n_registro->nota = nota;
}

/*
    Compara dos materias
    --------------------
    case insensitive, ignora tildes.
    Retorna:
        1: m1 > m2
        0: m1 = m2
        -1: m1 < m2
*/
int cmp_materias(char *m1, char *m2)
{
    const unsigned char *p1 = (const unsigned char *)m1;
    const unsigned char *p2 = (const unsigned char *)m2;
    int result;

    if (p1 == p2)
        return 0;

    // casteamos tolower a char para que compare bien los caracteres con tilde
    while ((result = (char)tolower(*p1) - (char)tolower(*p2++)) == 0)
        if (*p1++ == '\0')
            break;

    return result;
}

/*
    Compara dos t_registro
    ------------------
    Retorna:
        1: a > b
        0: a == b
        -1: a < b 
*/
int cmp_notas(t_registro a, t_registro b)
{
    // compara por dni
    if (a.dni > b.dni)
    {
        return 1;
    }
    else if (a.dni < b.dni)
    {
        return -1;
    }
    else // si los dni son iguales
    {
        // compara por materia
        if (cmp_materias(a.materia, b.materia) > 0)
        {
            return 1;
        }
        else if (cmp_materias(a.materia, b.materia) < 0)
        {
            return -1;
        }
        else // si las materias son iguales
        {
            // compara por tipo de evaluacion
            if (a.evaluacion > b.evaluacion)
            {
                return 1;
            }
            else if (a.evaluacion < b.evaluacion)
            {
                return -1;
            }
        }
    }

    return 0; // iguales
}

/*
    Inserta un nuevo nodo a la lista de notas
    -----------------------------------------
    Retorna:
        0: si no pudo insertar el valor porque ya existe
        1: si pudo insertar la nueva nota
*/
int insertar(Lista *lista, t_registro registro)
{
    pNodo nuevo, actual;

    // crear un nuevo nodo
    nuevo = (pNodo)malloc(sizeof(tipoNodo));
    nuevo->registro = registro;

    // colocamos actual en la primera posición de la lista
    actual = *lista;
    if (actual)
        while (actual->anterior)
            actual = actual->anterior;
    // si la lista está vacía o el primer miembro es mayor que el nuevo
    if (!actual || cmp_notas(actual->registro, registro) > 0)
    {
        // añadimos la lista a continuación del nuevo nodo
        nuevo->siguiente = actual;
        nuevo->anterior = NULL;
        if (actual)
            actual->anterior = nuevo;
        if (!*lista)
            *lista = nuevo;
    }
    else
    {
        // avanzamos hasta el último elemento o hasta que el siguiente tenga
        // un valor mayor que el nuevo
        while (actual->siguiente && cmp_notas(actual->siguiente->registro, registro) < 0)
            actual = actual->siguiente;

        // si el actual o el siguiente son iguales al nuevo
        if (cmp_notas(actual->registro, registro) == 0 ||
            (actual->siguiente && cmp_notas(actual->siguiente->registro, registro) == 0))
        {
            free(nuevo);

            return 0; // nodo repetido
        }

        // insertamos el nuevo nodo después del nodo anterior
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
        nuevo->anterior = actual;
        if (nuevo->siguiente)
            nuevo->siguiente->anterior = nuevo;
    }

    return 1;
}

/*
    Borra un nodo de la lista de notas
*/
void borrar(Lista *lista, t_registro registro)
{
    pNodo nodo;

    // buscar el nodo
    nodo = *lista;
    while (nodo && cmp_notas(nodo->registro, registro) < 0)
    {
        nodo = nodo->siguiente;
    }

    while (nodo && cmp_notas(nodo->registro, registro) > 0)
    {
        nodo = nodo->anterior;
    }

    // el valor no está en la lista
    if (!nodo || cmp_notas(nodo->registro, registro) != 0)
    {
        return;
    }

    // borrar el nodo
    // si lista apunta al nodo que queremos borrar, apuntar a otro
    if (nodo == *lista)
    {
        if (nodo->anterior)
        {
            *lista = nodo->anterior;
        }
        else
        {
            *lista = nodo->siguiente;
        }
    }

    if (nodo->anterior) // si no es el primer elemento
    {
        nodo->anterior->siguiente = nodo->siguiente;
    }
    if (nodo->siguiente) // no es el último nodo
    {
        nodo->siguiente->anterior = nodo->anterior;
    }

    free(nodo);
}

/*
    Liberar los recursos utilizados por la lista de notas
*/
void vaciar_lista(Lista *lista)
{
    pNodo nodo, actual;

    if (*lista != NULL)
    {
        actual = *lista;
        while (actual->anterior)
        {
            actual = actual->anterior;
        }

        while (actual)
        {
            nodo = actual;
            actual = actual->siguiente;
            free(nodo);
        }

        *lista = NULL;
    }
}

/*
    Guardar nueva nota en el archivo
    --------------------------------
    No se debe llamar antes de inicializar_notas
    Vuelca la nota en el archivo de notas de manera desordenada (siempre al final)
    Esto ayuda a ganar velocidad al momento de insertar, ya que si quisieramos
    mantener el archivo ordenado habría que volcar la lista completa en cada inserción.
    En el llamado a inicializar_notas, se reordena todo el archivo para mantenerlo
    "pseudo ordenado", es decir solamente ordenado para notas anteriores al momento
    de la ejecución en curso.
    Retorna:
        0: Error al abrir el archivo de notas para escritura
        1: Si la nota pudo guardase correctamente
*/
int guardar_nota_archivo(t_registro registro)
{
    FILE *f_notas = fopen(G_PATH_NOTAS, "a");

    if (f_notas == NULL)
    {
        // si ocurrió un error al abrir el archivo
        return 0;
    }

    fprintf(f_notas, "%d,%s,%d,%.2f\n", registro.dni, registro.materia, registro.evaluacion, registro.nota);

    fclose(f_notas);

    return 1;
}

//  ========================================================
//      funciones que pueden ser llamadas por el usuario
//  ========================================================

/*
    ver notas.h
*/
int inicializar_notas(char *path_notas)
{
    FILE *f_notas;           // archivo de notas
    char linea[1024];        // contiene cada registro
    char *delimitador = ","; // delimitardor de campos
    char *campo;             // valor del campo leido
    t_registro registro;     // registro leido
    int nro_campo;           // número de campo cargado
    pNodo nodo;              // para recorrer la lista cargada

    // setear el path de notas para uso global
    G_PATH_NOTAS = path_notas;

    // se abre para lectura
    f_notas = fopen(path_notas, "r");
    if (f_notas != NULL)
    {
        // inicializar lista de notas con las del archivo
        while (fgets(linea, 1024, f_notas))
        {
            // remover último salto de linea
            if (linea[strlen(linea) - 1] == '\n')
            {
                linea[strlen(linea) - 1] = '\0';
            }

            // lectura campo a campo
            campo = linea;
            for (nro_campo = 1; (campo = strtok(campo, delimitador)) != NULL; nro_campo++)
            {
                // setear valores del registro al formato de nodo de lista
                switch (nro_campo)
                {
                case 1:
                    registro.dni = atoi(campo);
                    break;
                case 2:
                    strcpy(registro.materia, campo);
                    break;
                case 3:
                    registro.evaluacion = atoi(campo);
                    break;
                case 4:
                    registro.nota = atof(campo);
                    break;
                }

                campo = NULL;
            }

            // agregar nuevo registro a la lista
            // se asume que siempre es correcto, por enunciado de ejercicio
            insertar(&l_notas, registro);
        }

        fclose(f_notas); // archivo cargado a memoria
    }

    // crear archivo de notas
    f_notas = fopen(path_notas, "w");
    if (f_notas != NULL)
    {
        // Una vez leido volcamos la lista al archivo
        // esto nos ayuda a matener el archivo limpio de repetidos
        // y pseudo ordenado (ordena los registros previos al momento de la ejecución en curso)
        if (l_notas)
        {
            nodo = l_notas;
            while (nodo->anterior)
            {
                nodo = nodo->anterior;
            }

            while (nodo)
            {
                fprintf(f_notas, "%d,%s,%d,%.2f\n", (*nodo).registro.dni, (*nodo).registro.materia, (*nodo).registro.evaluacion, (*nodo).registro.nota);
                nodo = nodo->siguiente;
            }
        }

        fclose(f_notas);
    }
    else
    {
        return 0; // no se pudo escribir en el archivo de notas
    }

    return 1; // notas cargadas
}

/*
    ver notas.h
*/
int cargar_nota(int dni, char *materia, enum TIPOS_EVALUACION evaluacion, float nota)
{
    t_registro nueva_nota;

    if (dni <= 0)
    {
        return 2; // DNI debe ser un número positivo
    }
    else
    {
        // remover espacios vacios al comienzo de la cadena
        while (isspace(*materia))
            ++materia;

        // verificar que la materia, no este vacia ni sea grande ni tenga comas
        if (materia[0] == '\0' || materia[0] == '\n')
        {
            return 3; // materia vacia
        }
        else if (strlen(materia) > 1000)
        {
            return 4; // materia demasiado larga
        }
        else if (strchr(materia, ','))
        {
            return 5; // la materia no puede contener comas
        }
        else if (evaluacion != PARCIAL_UNO &&
                 evaluacion != PARCIAL_DOS &&
                 evaluacion != RECUPERATORIO)
        {
            return 6; // tipo de evaluación no aceptado
        }
        else if (nota < 1 || nota > 10)
        {
            return 7; // nota fuera de rango
        }
        else
        {
            // inicializar valores del nuevo nodo
            init_nota(&nueva_nota, dni, materia, evaluacion, nota);

            if (insertar(&l_notas, nueva_nota)) // nueva nota insertada en memoria
            {
                if (guardar_nota_archivo(nueva_nota))
                {
                    return 1; // guardado correctamente en archivo
                }
                else // no se pudo guardar el registro en el archivo
                {
                    // la quitamos del listado de notas en memoria
                    borrar(&l_notas, nueva_nota);

                    return 9; // no se pudo guardar el registro en el archivo
                }
            }
            else // el registro está repetido
            {
                return 8;
            }
        }
    }

    return 1; // nota cargada
}

/*
    ver notas.h
*/
float get_promedio(int dni, char *materia)
{
    pNodo nodo = l_notas;     // puntero a listado de notas
    float promedio = 0;       // promedio de las notas según algoritmo
    int cantidad_notas = 0;   // auxiliar cantidad de notas leidas
    float parcial_uno = 0;    // nota parcial uno
    float parcial_dos = 0;    // nota parcial dos
    float recuperatorio = 0;  // nota recuperatorio
    float nota_mayor_uno = 0; // auxiliar nota más grande uno
    float nota_mayor_dos = 0; // auxiliar nota más grande dos

    if (l_notas)
    {
        while (nodo->anterior)
            nodo = nodo->anterior;
        while (nodo)
        {
            if (nodo->registro.dni == dni && cmp_materias(nodo->registro.materia, materia) == 0)
            {
                switch (nodo->registro.evaluacion)
                {
                case PARCIAL_UNO: // nota del primer parcial encontrada
                    parcial_uno = nodo->registro.nota;
                    cantidad_notas++;

                    // verificar si es una de sus dos mayores notas
                    if (nota_mayor_uno == 0 || parcial_uno > nota_mayor_uno)
                    {
                        nota_mayor_dos = nota_mayor_uno;
                        nota_mayor_uno = parcial_uno;
                    }

                    break;
                case PARCIAL_DOS: // nota del segundo parcial encontrada
                    parcial_dos = nodo->registro.nota;
                    cantidad_notas++;

                    // verificar si es una de sus dos mayores notas
                    if (nota_mayor_uno == 0 || parcial_dos > nota_mayor_uno)
                    {
                        nota_mayor_dos = nota_mayor_uno;
                        nota_mayor_uno = parcial_dos;
                    }

                    break;
                case RECUPERATORIO: // nota del recuperatorio encontrada
                    recuperatorio = nodo->registro.nota;
                    cantidad_notas++;

                    // verificar si es una de sus dos mayores notas
                    if (nota_mayor_uno == 0 || recuperatorio > nota_mayor_uno)
                    {
                        nota_mayor_dos = nota_mayor_uno;
                        nota_mayor_uno = recuperatorio;
                    }

                    break;
                }
            }
            else if (cantidad_notas > 0)
            {
                // como en memoria los datos están ordenados
                // por dni y por materia
                // no es necesario recorrer todo el listado
                // si ya se encontró al menos una nota y el dni nuevo es
                // diferente al del alumno buscado ó estamos en otra materia
                break;
            }

            nodo = nodo->siguiente;
        }

        // si no tiene más de una nota se considera que la materia no fue rendida
        if (cantidad_notas > 1)
        {
            if (cantidad_notas == 2) // si tiene dos notas
            {
                // al menos una de las tres es cero
                // por lo que podemos sumar todas
                // y retornar el promedio entre dos
                return (parcial_uno + parcial_dos + recuperatorio) / 2;
            }
            else if (cantidad_notas == 3) // se checkea para prevenir un error (más de 3 notas)
            {
                // retornamos el promedio entre las dos notas de mayor valor
                return (nota_mayor_uno + nota_mayor_dos) / 2;
            }
        }
    }

    return 0;
}

/*
    ver notas.h
*/
float get_promedio_general(int dni)
{
    pNodo nodo = l_notas;               // listado de notas
    char *materia;                      // materia de la que se encontró una nota
    float materia_promedio = 0;         // promedio  de la materia
    float sumatoria_promedios = 0;      // sumatoria de promedios
    int cantidad_materias_rendidas = 0; // aumenta en uno cuando materia_promedio > 0

    if (l_notas)
    {
        while (nodo->anterior)
            nodo = nodo->anterior;
        while (nodo)
        {
            if (nodo->registro.dni == dni) // alumno encontrado
            {
                // calcular el promedio para cada una de sus materias
                while (nodo && nodo->registro.dni == dni)
                {
                    materia = nodo->registro.materia;
                    materia_promedio = get_promedio(nodo->registro.dni, nodo->registro.materia);
                    if (materia_promedio > 0)
                    {
                        // si el alumno rindió la materia
                        sumatoria_promedios += materia_promedio;
                        cantidad_materias_rendidas++;
                    }

                    // avanzar a la siguiente materia rendida por el alumno
                    while (nodo && nodo->registro.dni == dni && cmp_materias(materia, nodo->registro.materia) == 0)
                    {
                        nodo = nodo->siguiente;
                    }

                    if (nodo && nodo->registro.dni == dni && cmp_materias(materia, nodo->registro.materia) != 0)
                    {
                        // si encontramos otra nota de una materia del alumno, continuamos
                        continue;
                    }

                    // si ya no queda ninguna nota, ó
                    // si ya no quedan notas del alumno buscado
                    break;
                }

                break;
            }

            nodo = nodo->siguiente;
        }
    }

    if (cantidad_materias_rendidas > 0)
    {
        return sumatoria_promedios / cantidad_materias_rendidas;
    }

    return 0;
}

/*
    ver notas.h
*/
void mostrar_notas(int orden)
{
    Lista lista = l_notas;
    pNodo nodo = lista;

    if (!lista)
    {
        printf("listado de notas vacío\n");
        return;
    }

    nodo = lista;
    if (orden == ASCENDENTE)
    {
        while (nodo->anterior)
            nodo = nodo->anterior;
        printf("Listado ascendente (dni/materia/evaluación/nota):\n");
        while (nodo)
        {
            mostrar_registro(&nodo->registro);
            nodo = nodo->siguiente;
        }
    }
    else
    {
        while (nodo->siguiente)
            nodo = nodo->siguiente;
        printf("Listado descendente (dni/materia/evaluación/nota):\n");
        while (nodo)
        {
            mostrar_registro(&nodo->registro);
            nodo = nodo->anterior;
        }
    }

    printf("-------------------------\n\n");
}

/*
    ver notas.h
*/
void liberar_memoria(void)
{
    vaciar_lista(&l_notas);
}