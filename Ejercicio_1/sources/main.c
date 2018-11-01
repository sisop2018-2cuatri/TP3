#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define TIPO(y, x) ((y == 'Z') ? "ZOMBIE" : (y == 'D') ? "DEMONIO" : ((x == 1) ? "PADRE" : (x == 2) ? "HIJO" : (x == 3) ? "NIETO" : "BISNIETO"))

int generacion = 1;       // generación del proceso
int cantidad_hijos = 2;   // cantidad de hijos que tiene el proceso
int cantidad_zombies = 2; // cantidad de zombies que tiene el proceso
int numero_hermanos = 1;  // cantidad de hermanos que son en total en esta generación

// mostrar datos del proceso en curso
void mostrarEntidad(char tipo);

// crear procesos hijos
void crearHijos();

int main()
{
    printf("\n*******************************************\n");
    printf("| Para cerrar los procesos presione ENTER |\n");
    printf("*******************************************\n\n");

    mostrarEntidad('G');

    // iniciar procreación
    crearHijos();

    // esperar una tecla (se ejecuta en todos los hijos)
    // esto lo hacemos para mantenerlos en memoria y
    // poder usar "pstree"
    getchar();
    printf("proceso [%d] finalizado\n", getpid());

    return 0;
}

void mostrarEntidad(char tipo)
{
    printf("pid[%d] ppid[%d] generación[%d] tipo[%s]\n",
           getpid(),
           getppid(),
           generacion,
           TIPO(tipo, generacion));
}

void crearHijos()
{
    int hijo_pid;

    // los procesos hijos de este proceso
    // tendrán "numero_hermanos" procesos hermanos
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
            mostrarEntidad('G');

            // solamente el primer proceso tiene hijos zombie
            cantidad_zombies = 0;

            // si aún no es la última generación
            if (generacion < 4)
            {
                // la cantidad de hijos que tiene, depende del número de
                // hermanos que tenga el proceso
                cantidad_hijos = cantidad_hijos == numero_hermanos ? 1 : 2;

                // crear hijos del proceso hijo
                crearHijos();
            }

            // como es el hijo, salimos del while
            // porque el while fue creado en el padre
            break;
        }

        // indicar que el hijo fue creado y crear el siguiente si lo hay
        cantidad_hijos--;
    }

    // crear zombies
    while (cantidad_zombies)
    {
        // crear nuevo zombie
        hijo_pid = fork();

        // si es el proceso hijo
        if (hijo_pid == 0)
        {
            // mostrar datos del proceso zombie
            mostrarEntidad('Z');

            // TODO: crear demonios

            printf("proceso [%d] finalizado para ser zombie\n", getpid());
            exit(0); // finalizar para que sea zombie
        }

        // indicar que el zombie fue creado y crear el siguiente si lo hay
        cantidad_zombies--;
    }
}