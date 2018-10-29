
/*
 *EJERCICIO 5
 *Git: https://github.com/sisop2018-2cuatri/TP3/Ejercicio_5
 *Programa: cliente.c
 *Descripcion: Cliente que permite conectar a un profesor al servidor de notas *universitario
 *Autor: maximiliano-rdl, LautaroLorenz
 *Fecha: 29/10/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cliente.h>
#include <config_loader.h>
#include <defines.h>
#include <unistd.h>
#include <print_utils.h>
#include <ctype.h>


int main(){
    s_config configuracion;
    int fd;

    cargar_config(&configuracion);

    if (configuracion.modo_ejecucion == 1){
        printf("MODO DEBUG ACTIVADO:\n\nSETTINGS:\n");
        mostrar_config(&configuracion); // mostrar valores de configuración
    }

    if (configuracion.mostrar_ayuda == 1){
        printf("\nAYUDA:\n");
        mostrar_ayuda(); // si hay que mostrar la ayuda en el inicio
    }

    // inicializar la conexión
    //inicializar_conexion();
    
    // menú de opciones
    mostrar_menu();
    
    return 1;
}


//ver cliente.h
void mostrar_ayuda(void)
{
    char *mensaje_ayuda =
        "SISTEMA DE NOTAS DE ALUMNOS\n"
        "---------------------------\n"
        "Se puede configurar utilizando el archivo\n"
        "de configuración \"./conf/config.conf\"\n"
        "Para aplicar los cambios guarde el archivo y luego\n"
        "reinicie la ejecución del servidor.\n"
        "----------------------------\n"
        "\n";

    printf("%s", mensaje_ayuda);
}


//ver cliente.h
void mostrar_menu(void)
{
    char opcion; // opción seleccionada
    int ch;      // para limpiar el flujo de entrada

    do
    {
        printf("Opciones: \n");
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


//ver cliente.h
void cargar_nueva_nota(void)
{
    printf("TODO: cargar nueva nota\n");
    // TODO:
}


//ver cliente.h
void obtener_promedio_materia(void)
{
    printf("TODO: obtener promedio de materia");
    /*int estado;           // estado de la respuesta
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
        if (strcmp(respuesta, "0.00") == 0)
        {
            printf("El alumno no rindió la materia [%s]\n", configuracion.materia_profesor);
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
    */
}


//ver cliente.h
void obtener_promedio_general(void)
{
    printf("Obtener promedio general");
    /*int estado;           // estado de la respuesta
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
            printf("alumno [%d]\n", dni);
            printf("promedio general [%s]\n", respuesta);
        }
    }

    printf("presione enter para continuar...\n");
    getchar();
    */
}


//ver cliente.h
void cambiar_la_conexion(void)
{
    printf("TODO?:Cambiar conexcion");
    /*
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
    */
}


//ver cliente.h
void borrar_datos_conexion(void)
{
    printf("TODO?: borrar datos conexion");
    /*
    modificar_variable("SERVER_DIRECCION", "");
    modificar_variable("SERVER_PUERTO", "");
    modificar_variable("MATERIA_PROFESOR", "");
    */
}

void finalizar_conexion(){};