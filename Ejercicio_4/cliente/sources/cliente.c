#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "../headers/cliente.h"
#include "../headers/configuracion.h"
#include "../headers/conexion.h"

// valores de configuración general
static Configuracion configuracion;

int main()
{
    // inicializar variables de configuración
    get_configuracion(&configuracion);

    if (configuracion.modo_ejecucion == DEBUG)
    {
        // mostrar valores de configuración
        mostrar_configuracion(&configuracion);
    }

    if (configuracion.mostrar_ayuda)
    {
        // si hay que mostrar la ayuda en el inicio
        mostrar_ayuda();
    }

    // inicializar la conexión
    inicializar_conexion(
        configuracion.materia_profesor,
        configuracion.server_direccion,
        configuracion.socket_puerto,
        configuracion.recordar_ultima_conexion);

    // atrapar señal de fin de ejecución
    atrapar_seniales();

    // menú de opciones
    mostrar_menu();

    return 0;
}

/*
    ver cliente.h
*/
void mostrar_ayuda(void)
{
    char *mensaje_ayuda =
        "SISTEMA DE NOTAS DE ALUMNOS\n"
        "---------------------------\n"
        "- Se puede configurar utializando el archivo\n"
        "de configuración \"./inputs/configuracion.txt\"\n"
        "Para aplicar los cambios guarde el archivo y luego\n"
        "reinicie la ejecución del cliente.\n"
        "- Para salir puede utilizar las señales SIGTERM ó SIGINT\n"
        "a menos que no puedan ser procesadas en su sistema cuyo\n"
        "caso será informado en la terminal.\n"
        "----------------------------\n"
        "\n";

    printf("%s", mensaje_ayuda);
}

/*
    ver cliente.h
*/
void mostrar_menu(void)
{
    char opcion; // opción seleccionada
    int ch;      // para limpiar el flujo de entrada

    do
    {
        printf("\n");
        printf("1] Cargar nueva nota\n");
        printf("2] Obtener promedio en la materia\n");
        printf("3] Ver promedio general del alumno\n");
        printf("4] Cambiar la conexión\n");
        printf("\n");
        printf("5] Borrar datos de conexión y salir\n");
        printf("6] Salir\n");

        do
        {
            printf("\nIntroduzca una opción (1-6): ");
            fflush(stdin);
            scanf(" %c", &opcion);

            while ((ch = getchar()) != EOF && ch != '\n')
            {
                // borrar todo lo que no es el primer caracter
                // osea únicamente nos intereza la opción
            }

            if (opcion < '1' || opcion > '6')
            {
                printf("Opción incorrecta\n");
            }
        } while (opcion < '1' || opcion > '6');

        printf("\n");
        switch (opcion)
        {
        case '1': // Cargar nueva nota
            cargar_nueva_nota();
            break;
        case '2': // Obtener promedio en la materia
            obtener_promedio_materia();
            break;
        case '3': // Ver promedio general del alumno
            obtener_promedio_general();
            break;
        case '4': // Cambiar la conexión
            cambiar_la_conexion();
            break;
        case '5': // Borrar mis datos y salir
            borrar_datos_conexion();
            break;
        default:
            // fin menú
            break;
        }

    } while (opcion != '5' && opcion != '6');

    // cerrar la conexión
    finalizar_conexion();
}

/*
    ver cliente.h
*/
void atrapar_seniales()
{
    if (signal(SIGTERM, fin_ejecucion) == SIG_ERR)
    {
        printf("ADVERTENCIA: no se podrá procesar SIGTERM\n");
        printf("evite utilizar esta señal\n");
    }
    if (signal(SIGINT, fin_ejecucion) == SIG_ERR)
    {
        printf("ADVERTENCIA: no se podrá procesar SIGINT\n");
        printf("evite utilizar esta señal\n");
    }

    // SIGKILL y SIGSTOP nunca pueden atraparse
}

/*
    ver cliente.h
*/
void fin_ejecucion(int senial)
{
    printf("\n");

    // cerrar la conexión
    finalizar_conexion();

    // salir del programa
    exit(0);
}

/*
    ver cliente.h
*/
void mensaje_materia_rendida(void)
{
    printf("Se considera que un alumno rindió una materia\n");
    printf("cuando se le hayan cargado al menos dos notas\n");
}

//  ====================================
//          FUNCIONES DEL MENÚ
//  ====================================

/*
    ver cliente.h
*/
void cargar_nueva_nota(void)
{
    int estado;           // estado de la respuesta
    char mensaje[1024];   // mensaje que se envia al servidor
    char respuesta[1024]; // respuesta del servidor
    int c_buffer;         // para limpiar el buffer del teclado
    int dni;              // dni del alumno
    float nota;           // nota del alumno en la evaluación
    int tipo_evaluacion;  // tipo de evaluación

    printf("cargar nota al alumno (dni): ");
    while (scanf("%i", &dni) != 1)
    {
        printf("ERROR: dni no aceptado\n");
        while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
        {
            // limpiar buffer
        }
        printf("Ingrese dni del alumno: ");
    }
    while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
    {
        // limpiar buffer
    }

    printf("\ntipos de evaluación\n");
    printf("\t1: primer parcial\n");
    printf("\t2: segundo parcial\n");
    printf("\t3: recuperatorio\n");
    printf("Ingrese tipo de evaluación (1-3): ");
    while (scanf("%i", &tipo_evaluacion) != 1 || tipo_evaluacion < 1 || tipo_evaluacion > 3)
    {
        printf("ERROR: tipo de evaluación no aceptado\n");
        while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
        {
            // limpiar buffer
        }
        printf("Ingrese tipo de evaluación (1-3): ");
    }
    while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
    {
        // limpiar buffer
    }
    // como los tipos de evaluación van de 0 a 2
    tipo_evaluacion--;

    printf("Ingrese la nota (1 a 10): ");
    while (scanf("%f", &nota) != 1 || nota < 1 || nota > 10)
    {
        printf("ERROR: nota no aceptada\n");
        while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
        {
            // limpiar buffer
        }
        printf("Ingrese la nota (1 a 10): ");
    }
    while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
    {
        // limpiar buffer
    }

    // asignar valores al mensaje
    mensaje[0] = '\0';
    sprintf(mensaje,
            "cargar_nota,%d,%s,%d,%.2f",
            dni,
            configuracion.materia_profesor,
            tipo_evaluacion,
            nota);

    // hacer solicitud al servidor
    respuesta[0] = '\0';
    estado = enviar_mensaje(mensaje, respuesta);
    if (estado != 1)
    {
        if (estado == 2)
        {
            printf("ERROR: no se pudo enviar la solicitud al servidor\n");
        }
        else if (estado == 3)
        {
            printf("ERROR: no se pudo recibir respuesta del servidor\n");
        }
    }
    else
    {
        printf("\n");
        estado = atoi(respuesta);
        switch (estado)
        {
        case 1:
            printf("alumno [%d]\n", dni);
            printf("materia [%s]\n", configuracion.materia_profesor);
            printf("evaluación [%s]\n",
                   (tipo_evaluacion == 0) ? "PARCIAL UNO" : ((tipo_evaluacion == 1) ? "PARCIAL DOS" : "RECUPERATORIO"));
            printf("nota [%.2f]\n", nota);
            printf("Cargada correctamente\n");
            break;
        case 2:
            printf("ERROR: dni debe ser un número positivo mayor a cero\n");
            break;
        case 3:
            printf("ERROR: materia vacía\n");
            break;
        case 4:
            printf("ERROR: materia demasiado larga\n");
            break;
        case 5:
            printf("ERROR: materia no puede contener caracter coma (,)\n");
            break;
        case 6:
            printf("ERROR: tipo de evaluación incorrecto\n");
            break;
        case 7:
            printf("ERROR: valores de nota fuera de rango (debe ser mayor a 0 y menor que 11)\n");
            break;
        case 8:
            printf("ERROR: nota ya cargada previamente\n");
            break;
        case 9:
            printf("ERROR: error al guardar de manera permanente\n");
            break;
        default:
            printf("ERROR: el servidor emitió una respuesta desconocida\n");
            break;
        }
    }

    printf("presione enter para continuar...\n");
    getchar();
}

/*
    ver cliente.h
*/
void obtener_promedio_materia(void)
{
    int estado;           // estado de la respuesta
    char mensaje[1024];   // mensaje que se envia al servidor
    char respuesta[1024]; // respuesta del servidor
    int c_buffer;         // para limpiar el buffer del teclado
    int dni;              // dni del alumno

    printf("obtener promedio de la materia del alumno (dni): ");
    while (scanf("%i", &dni) != 1)
    {
        printf("ERROR: dni no aceptado\n");
        while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
        {
            // limpiar buffer
        }
        printf("Ingrese dni del alumno: ");
    }
    while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
    {
        // limpiar buffer
    }

    // asignar valores al mensaje
    mensaje[0] = '\0';
    sprintf(mensaje, "get_promedio,%d,%s", dni, configuracion.materia_profesor);

    // hacer solicitud al servidor
    respuesta[0] = '\0';
    estado = enviar_mensaje(mensaje, respuesta);
    if (estado != 1)
    {
        if (estado == 2)
        {
            printf("ERROR: no se pudo enviar la solicitud al servidor\n");
        }
        else if (estado == 3)
        {
            printf("ERROR: no se pudo recibir respuesta del servidor\n");
        }
    }
    else
    {
        printf("\n");
        if (strcmp(respuesta, "0.00") == 0)
        {
            printf("El alumno [%d] no rindió la materia [%s]\n", dni, configuracion.materia_profesor);
            mensaje_materia_rendida();
        }
        else if (strcmp(respuesta, "-1") == 0)
        {
            printf("ERROR: no se pudo obtener promedio del alumno\n");
        }
        else
        {
            printf("alumno [%d]\n", dni);
            printf("materia [%s]\n", configuracion.materia_profesor);
            printf("promedio [%s]\n", respuesta);
        }
    }

    printf("presione enter para continuar...\n");
    getchar();
}

/*
    ver cliente.h
*/
void obtener_promedio_general(void)
{
    int estado;           // estado de la respuesta
    char mensaje[1024];   // mensaje que se envia al servidor
    char respuesta[1024]; // respuesta del servidor
    int c_buffer;         // para limpiar el buffer del teclado
    int dni;              // dni del alumno

    printf("obtener promedio general del alumno (dni): ");
    while (scanf("%i", &dni) != 1)
    {
        printf("ERROR: dni no aceptado\n");
        while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
        {
            // limpiar buffer
        }
        printf("Ingrese dni del alumno: ");
    }
    while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
    {
        // limpiar buffer
    }

    // asignar valores al mensaje
    mensaje[0] = '\0';
    sprintf(mensaje, "get_promedio_general,%d", dni);

    // hacer solicitud al servidor
    respuesta[0] = '\0';
    estado = enviar_mensaje(mensaje, respuesta);
    if (estado != 1)
    {
        if (estado == 2)
        {
            printf("ERROR: no se pudo enviar la solicitud al servidor\n");
        }
        else if (estado == 3)
        {
            printf("ERROR: no se pudo recibir respuesta del servidor\n");
        }
    }
    else
    {
        printf("\n");
        if (strcmp(respuesta, "0.00") == 0)
        {
            printf("El alumno [%d] no rindió ninguna materia\n", dni);
            mensaje_materia_rendida();
        }
        else if (strcmp(respuesta, "-1") == 0)
        {
            printf("ERROR: no se pudo obtener promedio general del alumno\n");
        }
        else
        {
            printf("alumno [%d]\n", dni);
            printf("promedio general [%s]\n", respuesta);
        }
    }

    printf("presione enter para continuar...\n");
    getchar();
}

/*
    ver cliente.h
*/
void cambiar_la_conexion(void)
{
    // limpiar parámetros de conexión
    configuracion.materia_profesor[0] = '\0';
    configuracion.server_direccion[0] = '\0';
    configuracion.socket_puerto = 0;

    // solicitar nueva conexión
    inicializar_conexion(
        configuracion.materia_profesor,
        configuracion.server_direccion,
        configuracion.socket_puerto,
        configuracion.recordar_ultima_conexion);
}

/*
    ver cliente.h
*/
void borrar_datos_conexion(void)
{
    modificar_variable("SERVER_DIRECCION", "");
    modificar_variable("SERVER_PUERTO", "");
    modificar_variable("MATERIA_PROFESOR", "");
}