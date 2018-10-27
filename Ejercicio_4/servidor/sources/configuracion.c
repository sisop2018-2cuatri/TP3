#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/configuracion.h"

/*
    Inicializa archivo de configuración con los valores default
*/
void inicializar_archivo(FILE *f_config)
{
    char *default_config =
        "// Archivo de configuración\n"
        "// los valores se toman desde el caracter '=' hasta el salto de linea\n"
        "// toda linea que comienza con // es ignorada\n"
        "// ------------------------\n"
        "\n"
        "// modo de ejecución (int):\n"
        "//     0==RELEASE\n"
        "//     1==DEBUG\n"
        "// en modo DEBUG se emiten mensajes de estado\n"
        "// en modo RELEASE únicamente mensajes de error\n"
        "MODO_EJECUCION=DEBUG\n"
        "\n"
        "// path archivo de notas (char[1000]):\n"
        "ARCHIVO_NOTAS=./inputs/notas.txt\n"
        "\n"
        "// mostrar ayuda en pantalla al iniciar (int <0||1>):\n"
        "//     0==FALSE\n"
        "//     1==TRUE\n"
        "MOSTRAR_AYUDA=TRUE\n"
        "\n"
        "// (int) puerto utilizado por el socket\n"
        "// debe ser mayor a 1024 y menor a 65536\n"
        "// por seguridad se recomienda usar valores altos\n"
        "SOCKET_PUERTO=55444\n"
        "\n"
        "// (int > 0) cantidad máxima de clientes\n"
        "// que puede contener el buffer del socket servidor\n"
        "CANTIDAD_CLIENTES_MAXIMA=10\n"
        "";

    fprintf(f_config, "%s", default_config);
}

/*
    Inicializar valores de configuración leidos desde un archivo
*/
void inicializar_desde_archivo(FILE *f_config, Configuracion *configuracion)
{
    char linea[1024]; // contiene cada par key=value
    char *key;
    char *value;
    char *delimitador = "="; // delimitardor de key=value

    while (fgets(linea, 1024, f_config))
    {
        // ignorar lineas que comienzan con "//"
        if (linea[0] == '/' && linea[1] == '/')
        {
            continue;
        }

        // ignorar lineas vacias
        if (linea[0] == '\0' || linea[0] == '\n')
        {
            continue;
        }

        // obtener de la linea cada par key=value
        key = strtok(linea, delimitador);
        value = strtok(NULL, delimitador);

        // remover último salto de linea
        if (value[strlen(value) - 1] == '\n')
        {
            value[strlen(value) - 1] = '\0';
        }

        if (strcasecmp(key, "MODO_EJECUCION") == 0)
        {
            if (strcasecmp(value, "DEBUG") == 0)
            {
                configuracion->modo_ejecucion = 1;
            }
            else if (strcasecmp(value, "RELEASE") == 0)
            {
                configuracion->modo_ejecucion = 0;
            }
            else
            {
                configuracion->modo_ejecucion = atoi(value);
            }
        }
        else if (strcasecmp(key, "ARCHIVO_NOTAS") == 0)
        {
            strcpy(configuracion->path_notas, value);
        }
        else if (strcasecmp(key, "MOSTRAR_AYUDA") == 0)
        {
            if (strcasecmp(value, "TRUE") == 0)
            {
                configuracion->mostrar_ayuda = 1;
            }
            else if (strcasecmp(value, "FALSE") == 0)
            {
                configuracion->mostrar_ayuda = 0;
            }
            else
            {
                configuracion->mostrar_ayuda = atoi(value);
            }
        }
        else if (strcasecmp(key, "SOCKET_PUERTO") == 0)
        {
            configuracion->socket_puerto = atoi(value);
        }
        else if (strcasecmp(key, "CANTIDAD_CLIENTES_MAXIMA") == 0)
        {
            configuracion->cantidad_clientes_maxima = atoi(value);
        }
    }
}

/*
    ver configuracion.h
*/
void get_configuracion(Configuracion *configuracion)
{
    FILE *f_config;

    // carga de valores default
    // ------------------------
    configuracion->modo_ejecucion = MODO_EJEC;              // modo de ejecución
    strcpy(configuracion->path_notas, PATH_NOTAS);          // archivo de notas
    configuracion->mostrar_ayuda = MOSTRAR_AYUDA;           // mostrar la ayuda en el inicio
    configuracion->socket_puerto = SOCKET_PUERTO;           // puerto del socket
    configuracion->cantidad_clientes_maxima = CANT_CLI_MAX; // cantidad de clientes en simultáneo como máximo

    // cargar ó crear archivo
    // ----------------------
    f_config = fopen("./inputs/configuracion.txt", "r");
    if (f_config != NULL) // si el archivo pudo leerse
    {
        // obtenemos los datos
        inicializar_desde_archivo(f_config, configuracion);

        fclose(f_config); // cerrar archivo
    }
    else // si el archivo no existe
    {
        // crear archivo
        f_config = fopen("./inputs/configuracion.txt", "w");
        if (f_config != NULL) // si pudo crearse el archivo
        {
            // guardar configuración default
            inicializar_archivo(f_config);

            fclose(f_config); // cerrar archivo
        }
        else // si no pudo crearse el archivo
        {
            printf("WARNING: el archivo de configuración no pudo crearse\n");
        }
    }
}

/*
    ver configuracion.h
*/
void mostrar_configuracion(Configuracion *configuracion)
{
    printf("VARIABLES DE CONFIGURACIÓN\n");
    printf("--------------------------\n");
    printf("MODO DE EJECUCIÓN: %s\n", (configuracion->modo_ejecucion == DEBUG) ? "DEBUG" : "RELEASE");
    printf("ARCHIVO DE NOTAS: \"%s\"\n", configuracion->path_notas);
    printf("MOSTRAR AYUDA: %s\n", (configuracion->mostrar_ayuda == 1) ? "TRUE" : "FALSE");
    printf("SOCKET PUERTO: %d\n", configuracion->socket_puerto);
    printf("CANTIDAD MÁXIMA DE CLIENTES: %d\n", configuracion->cantidad_clientes_maxima);
    printf("--------------------------\n\n");
}