#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "../headers/servidor.h"
#include "../headers/conexion.h"
#include "../headers/configuracion.h"
#include "../headers/notas.h"

int main()
{
    int estado;   // guarda el estado de las funciones
    int c_buffer; // para limpiar el buffer del teclado

    // valores de configuración general
    Configuracion configuracion;

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

    // inicializar archivo de notas
    if (inicializar_notas(configuracion.path_notas) != 1)
    {
        printf("ERROR: no se pudo cargar el archivo de notas, intente con un path diferente\n");
    }
    else
    {
        if (configuracion.modo_ejecucion == DEBUG)
        {
            printf("archivo de notas cargado correctamente\n");
            mostrar_notas(ASCENDENTE);
        }

        // inicializar la conexión
        estado = inicializar_conexion(
            configuracion.modo_ejecucion,
            configuracion.socket_puerto,
            configuracion.cantidad_clientes_maxima);

        if (estado != 1)
        {
            if (estado == 2) // error de conexión
            {
                printf("ERROR: no se pudo crear el socket\n");
                printf("Intente cambiando los valores de conexión en el archivo de configuración\n");
                printf("y volviendo a ejecutar el programa\n");
            }
            else if (estado == 3) // error de conexión
            {
                do
                {
                    printf("ERROR: no se pudo asociar el socket con el puerto [%d]\n", configuracion.socket_puerto);

                    // ingresar un nuevo puerto
                    printf("Ingrese puerto de conexión: ");
                    while (scanf("%i", &configuracion.socket_puerto) != 1 ||
                           configuracion.socket_puerto <= 1024 ||
                           configuracion.socket_puerto > 65535)
                    {
                        printf("\nERROR: el puerto debe ser mayor a 1024 y menor a 65536\n");
                        while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
                        {
                            // limpiar buffer
                        }
                        printf("Ingrese puerto de conexión: ");
                    }
                    while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
                    {
                        // limpiar buffer
                    }

                    // inicializar la conexión
                    estado = inicializar_conexion(
                        configuracion.modo_ejecucion,
                        configuracion.socket_puerto,
                        configuracion.cantidad_clientes_maxima);
                } while (estado != 1);
            }
            else if (estado == 4) // error de conexión
            {
                printf("ERROR: no se pudo configurar el socket en modo listen\n");
                printf("Intente cambiando los valores de conexión en el archivo de configuración\n");
                printf("y volviendo a ejecutar el programa\n");
            }
        }

        if (estado == 1)
        {
            // si se inicializó correctamente
            if (configuracion.modo_ejecucion == DEBUG)
            {
                printf("conexión inicializada\n");
                printf("atendiendo solicitudes de los clientes\n");
            }

            // atrapar señal de fin de ejecución
            atrapar_seniales();

            // comienza a escuchar solicitudes de los clientes
            atender_solicitudes();
        }
    }

    return 0;
}

/*
    ver servidor.h
*/
void mostrar_ayuda(void)
{
    char *mensaje_ayuda =
        "SERVIDOR DE NOTAS DE ALUMNOS\n"
        "----------------------------\n"
        "Se puede configurar utializando el archivo\n"
        "de configuración \"./inputs/configuracion.txt\"\n"
        "Para aplicar los cambios guarde el archivo y luego\n"
        "reinicie la ejecución del servidor.\n"
        "Para salir puede utilizar las señales SIGTERM ó SIGINT\n"
        "a menos que no puedan ser procesadas en su sistema cuyo\n"
        "caso será informado en la terminal.\n"
        "----------------------------\n"
        "\n";

    printf("%s", mensaje_ayuda);
}

/*
    ver servidor.h
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
    ver servidor.h
*/
void fin_ejecucion(int senial)
{
    printf("\n");

    // cerrar la conexión
    finalizar_conexion();

    // liberar recursos
    liberar_memoria();
    mostrar_notas(ASCENDENTE);

    // salir del programa
    exit(0);
}