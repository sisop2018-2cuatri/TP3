#include <stdio.h>
#include "../headers/servidor.h"
#include "../headers/conexion.h"
#include "../headers/configuracion.h"
#include "../headers/notas.h"

// TODO: atrapar SIGTERM y el otro SIG para cerrar los puertos (servidor y clientes)
// TODO: Compila sin errores con el script entregado
// TODO: Funciona correctamente según enunciado
// TODO: Existe algún tipo de ayuda para la ejecución del proceso
// TODO: No deja procesos zombies o huérfanos.
// TODO: Valida correctamente los parámetros
// TODO: Funciona tanto localmente como a través de la red desde distintas máquinas
// TODO: Utiliza sockets y threads
// TODO: Maneja correctamente el cierre de los puertos al finalizar
// TODO: Tiene archivo de configuración (deben entregarlo)
// TODO: Se conecta con nombre de máquinas
// TODO: Soporta múltiples clientes (se probará con más de 5)

int main()
{
    int estado; // guarda el estado de las funciones

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
            }
            else if (estado == 3) // error de conexión
            {
                printf("ERROR: no se pudo asociar el socket con el puerto\n");
            }
            else if (estado == 4) // error de conexión
            {
                printf("ERROR: no se pudo configurar el socket en modo listen\n");
            }
        }
        else
        {
            // si se inicializó correctamente
            if (configuracion.modo_ejecucion == DEBUG)
            {
                printf("conexión inicializada\n");
                printf("atendiendo solicitudes de los clientes\n");
            }

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
        "----------------------------\n"
        "\n";

    printf("%s", mensaje_ayuda);
}