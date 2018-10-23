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
        "// mostrar ayuda en pantalla al iniciar\n"
        "//     0==FALSE\n"
        "//     1==TRUE\n"
        "MOSTRAR_AYUDA=TRUE\n"
        "\n"
        "// dirección del servidor (char[1000]):\n"
        "SERVER_DIRECCION=127.0.0.1\n"
        "\n"
        "// (int) puerto de conexión al socket del servidor:\n"
        "SERVER_PUERTO=55444\n"
        "\n"
        "// si se puede conectar, recuerda la IP y puerto\n"
        "// modificando los valores de SERVER_DIRECCION y SERVER_PUERTO\n"
        "// en el próximo inicio intentará conectar automáticamente\n"
        "//     0==FALSE\n"
        "//     1==TRUE\n"
        "RECORDAR_ULTIMA_CONEXION=TRUE\n"
        "\n"
        "// materia dictada por el profesor (char[1000]):\n"
        "MATERIA_PROFESOR=\n"
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
        else if (strcasecmp(key, "SERVER_DIRECCION") == 0)
        {
            strcpy(configuracion->server_direccion, value);
        }
        else if (strcasecmp(key, "SERVER_PUERTO") == 0)
        {
            configuracion->socket_puerto = atoi(value);
        }
        else if (strcasecmp(key, "RECORDAR_ULTIMA_CONEXION") == 0)
        {
            if (strcasecmp(value, "TRUE") == 0)
            {
                configuracion->recordar_ultima_conexion = 1;
            }
            else if (strcasecmp(value, "FALSE") == 0)
            {
                configuracion->recordar_ultima_conexion = 0;
            }
            else
            {
                configuracion->recordar_ultima_conexion = atoi(value);
            }
        }
        else if (strcasecmp(key, "MATERIA_PROFESOR") == 0)
        {
            strcpy(configuracion->materia_profesor, value);
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
    configuracion->modo_ejecucion = MODO_EJEC;                          // modo de ejecución
    configuracion->mostrar_ayuda = MOSTRAR_AYUDA;                       // mostrar la ayuda en el inicio
    strcpy(configuracion->server_direccion, SERVER_IP);                 // dirección del servidor
    configuracion->socket_puerto = SERVER_PUERTO;                       // puerto del socket
    configuracion->recordar_ultima_conexion = RECORDAR_ULTIMA_CONEXION; // si se puede conectar, almacena la IP y puerto
    strcpy(configuracion->materia_profesor, MATERIA_PROFESOR);          // materia del profesor conectado

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
    printf("MOSTRAR AYUDA: %s\n", (configuracion->mostrar_ayuda == 1) ? "TRUE" : "FALSE");
    printf("DIRECCIÓN DEL SERVIDOR: \"%s\"\n", configuracion->server_direccion);
    printf("PUERTO DEL SOCKET: %d\n", configuracion->socket_puerto);
    printf("RECORDAR ÚLTIMA CONEXIÓN: %s\n", (configuracion->recordar_ultima_conexion == 1) ? "TRUE" : "FALSE");
    printf("MATERIA DEL PROFESOR: \"%s\"\n", configuracion->materia_profesor);
    printf("--------------------------\n\n");
}

/*
    ver configuracion.h
*/
int modificar_variable(char *nombre_variable, char *valor_nuevo)
{
    char linea[1024]; // contiene cada par key=value
    char *key;
    char *value;
    char *delimitador = "="; // delimitardor de key=value

    // flag de variable modificada vale uno si pudo modificarse
    int flag_variable_modificada = 0;

    // crear archivo temporal
    FILE *f_temp = fopen("./inputs/temporal.txt", "w");
    if (f_temp == NULL)
    {
        return 2; // ERROR: no se pudo crear el archivo temporal
    }

    // abrir archivo de configuracion
    FILE *f_config = fopen("./inputs/configuracion.txt", "r");
    if (f_config == NULL)
    {
        return 3; // ERROR: archivo de configuracion no encontrado
    }

    // por cada linea del archivo de configuración
    while (fgets(linea, 1024, f_config))
    {
        // si la variable ya fue modificada, transcribimos el resto del archivo
        if (flag_variable_modificada == 1)
        {
            fprintf(f_temp, "%s", linea);
            continue;
        }

        // ignorar lineas que comienzan con "//"
        if (linea[0] == '/' && linea[1] == '/')
        {
            fprintf(f_temp, "%s", linea);
            continue;
        }

        // ignorar lineas vacias
        if (linea[0] == '\0' || linea[0] == '\n')
        {
            fprintf(f_temp, "%s", linea);
            continue;
        }

        // obtener de la linea cada par key=value
        key = strtok(linea, delimitador);
        value = strtok(NULL, delimitador);

        // si es la linea que quiero editar
        if (strcasecmp(nombre_variable, key) == 0)
        {
            // variable encontrada
            fprintf(f_temp, "%s=%s\n", key, valor_nuevo);

            // indicar que la variable fue modificada
            flag_variable_modificada = 1;
        }
        else
        {
            // no era la variable que buscabamos
            // transcribimos tal cual
            fprintf(f_temp, "%s=%s", key, value);
        }
    }

    fclose(f_temp);
    fclose(f_config);

    // eliminar archivo de configuración viejo
    if (remove("./inputs/configuracion.txt") != 0)
    {
        return 4; // ERROR: no se pudo actualizar el archivo
    }
    else
    {
        if (rename("./inputs/temporal.txt", "./inputs/configuracion.txt") != 0)
        {
            return 4; // ERROR: no se pudo actualizar el archivo
        }
    }

    if (flag_variable_modificada == 0)
    {
        return 5; // variable no encontrada
    }

    return 1; // variable modificada
}