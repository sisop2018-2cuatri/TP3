#include <stdio.h>
#include <string.h>
#include "../headers/cliente.h"
#include "../headers/configuracion.h"
#include "../headers/conexion.h"

// TODO: on SIGTERM y el otro SIG, CERRAR EL PUERTO CORRECTAMENTE

// valores de configuración general
Configuracion configuracion;

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
        "Se puede configurar utializando el archivo\n"
        "de configuración \"./inputs/configuracion.txt\"\n"
        "Para aplicar los cambios guarde el archivo y luego\n"
        "reinicie la ejecución del servidor.\n"
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

//  ====================================
//          FUNCIONES DEL MENÚ
//  ====================================

/*
    ver cliente.h
*/
void cargar_nueva_nota(void)
{
    printf("cargar nueva nota\n");
    // TODO:
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

    printf("obtener promedio de la materia del alumno (dni): \n");
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

    if (configuracion.modo_ejecucion == DEBUG)
    {
        printf("dni ingresado [%d]\n", dni);
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
        // TODO: seguir desde acá
        /*if (strcmp(respuesta, "0.00") == 0)
        {
            printf("El alumno no rindió ninguna materia\n");
        }
        else if (strcmp(respuesta, "-1") == 0)
        {
            printf("ERROR: no se pudo obtener promedio general del alumno\n");
        }
        else
        {
            printf("El promedio general del alumno es [%s]\n", respuesta);
        }*/
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

    if (configuracion.modo_ejecucion == DEBUG)
    {
        printf("dni ingresado [%d]\n", dni);
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
        if (strcmp(respuesta, "0.00") == 0)
        {
            printf("El alumno no rindió ninguna materia\n");
        }
        else if (strcmp(respuesta, "-1") == 0)
        {
            printf("ERROR: no se pudo obtener promedio general del alumno\n");
        }
        else
        {
            printf("El promedio general del alumno es [%s]\n", respuesta);
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