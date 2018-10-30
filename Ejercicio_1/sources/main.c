#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define TIPO(x) ((x == 1) ? "PADRE" : (x == 2) ? "HIJO" : (x == 3) ? "NIETO" : "BISNIETO")

int generacion = 1;     // generación del proceso
int cantidad_hijos = 2; // cantidad de hijos que faltan crear en el proceso
int hijos_pid[100];     // pid de los hijos creados del proceso
int hijos_creados = 0;  // cantidad de hijos que se crearon en este proceso
int pid_padre;          // pid del proceso padre de todos

void mostrarEntidad();
void crearHijos();
void finalizarHijos();

int main()
{
    pid_padre = getpid(); // pid del proceso padre

    printf("\n*******************************\n");
    printf("| al finalizar presione ENTER |\n");
    printf("*******************************\n\n");

    mostrarEntidad();

    // iniciar procreación
    crearHijos();

    // escuchar sigkill
    signal(SIGINT, finalizarHijos);

    // esperar una tecla (se ejecuta en todos los hijos)
    // esto lo hacemos para mantenerlos en memoria y
    // poder usar "pstree"
    getchar();

    // si es el fin del proceso padre
    if (pid_padre == getpid())
    {
        // finalizar todos los hijos
        finalizarHijos();
    }

    return 0;
}

void finalizarHijos()
{
    int i;

    // matar todos los hijos
    for (i = 0; i < hijos_creados; i++)
    {
        kill(hijos_pid[i], SIGINT);
    }

    if (pid_padre == getpid())
    {
        printf("\n");
    }

    // finalizar proceso
    printf("proceso [%d] finalizado\n", getpid());

    if (pid_padre == getpid())
    {
        // para que la terminal espere a mostrar todos los printf
        // antes de poder a utilizar el stdin
        sleep(1);
    }

    exit(0);
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

            // el hijo aún no tiene hijos
            hijos_creados = 0;

            // mostrar datos del hijo
            mostrarEntidad();

            // si aún no es la última generación
            if (generacion < 4)
            {
                // si es el último hijo de la generacion
                if (cantidad_hijos == 1)
                {
                    // según la generación
                    // sabemos la cantidad de hijos que debe tener
                    cantidad_hijos = generacion == 2 ? 3 : 5;
                    crearHijos();
                }
            }

            // como es el hijo, salimos del while
            // porque el while fue creado en el padre
            break;
        }
        else
        {
            // guardar pid del hijo
            // para matarlo al finalizar proceso el proceso padre
            hijos_pid[hijos_creados++] = hijo_pid;
        }

        // indicar que el hijo fue creado y crear el siguiente si lo hay
        cantidad_hijos--;
    }
}