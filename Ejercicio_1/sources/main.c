#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define TIPO(x) ((x == 1) ? "PADRE" : (x == 2) ? "HIJO" : (x == 3) ? "NIETO" : "BISNIETO")

int generacion = 1;      // generación del proceso
int cantidad_hijos = 2;  // cantidad de hijos que hay en la generación siguiente
int numero_hermanos = 1; // cantidad de hermanos que tiene un proceso hijo
int hijos_pid[100];      // pid de los hijos creados del proceso

void mostrarEntidad();
void crearHijos();
void finalizarHijos();

int main()
{
    printf("\n*******************************************\n");
    printf("| Para cerrar los procesos presione ENTER |\n");
    printf("*******************************************\n\n");

    mostrarEntidad();

    // iniciar procreación
    crearHijos();

    // esperar una tecla (se ejecuta en todos los hijos)
    // esto lo hacemos para mantenerlos en memoria y
    // poder usar "pstree"
    getchar();
    printf("proceso [%d] finalizado\n", getpid());

    return 0;
}

void mostrarEntidad()
{
    printf("pid[%d] ppid[%d] generación[%d] tipo[%s]\n",
           getpid(),
           getppid(),
           generacion,
           TIPO(generacion));
}

void crearHijos()
{
    int hijo_pid;

    // los procesos hijos de este proceso
    // tendran "numero_hermanos" procesos hermanos
    numero_hermanos = cantidad_hijos * numero_hermanos - 1;

    // mientras tenga hijos para crear
    while (cantidad_hijos)
    {
        // crear nuevo hijo
        hijo_pid = fork();

        // si estamos en el proceso hijo
        if (hijo_pid == 0)
        {
            // el hijo es de la siguiente generación
            generacion++;

            // mostrar datos del hijo
            mostrarEntidad();

            // si aún no es la última generación
            if (generacion < 4)
            {
                // la cantidad de hijos que tiene, depende del número de
                // hermanos que tenga el proceso
                cantidad_hijos = cantidad_hijos == numero_hermanos ? 1 : 2;
                crearHijos();
            }

            // como es el hijo, salimos del while
            // porque el while fue creado en el padre
            break;
        }

        // indicar que el hijo fue creado y crear el siguiente si lo hay
        cantidad_hijos--;
    }
}