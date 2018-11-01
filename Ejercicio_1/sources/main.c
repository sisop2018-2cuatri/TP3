#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define TIPO(y, x) ((y == 'Z') ? "ZOMBIE" : (y == 'D') ? "DEMONIO" : ((x == 1) ? "PADRE" : (x == 2) ? "HIJO" : (x == 3) ? "NIETO" : "BISNIETO"))

int generacion = 1;        // generación del proceso
int cantidad_hijos = 2;    // cantidad de hijos que tiene el proceso
int cantidad_zombies = 2;  // cantidad de zombies que tiene el proceso
int cantidad_demonios = 0; // cantidad de demonios que tiene el proceso
int numero_hermanos = 1;   // cantidad de hermanos que son en total en esta generación

// mostrar datos del proceso en curso
void mostrarEntidad(char tipo);

// crear procesos hijos
void crearHijos();

// crear procesos demonios (solo pueden ser hijos de zombies)
// Retorna:
//  0: si es el proceso demonio creado
//  mayor a 0: si es el proceso que creó los demonios
int crearDemonios();

int main()
{
    printf("\n*********************************************\n");
    printf("| Para cerrar los procesos presione ENTER   |\n");
    printf("| Nota:                                     |\n");
    printf("|   Los zombie finalizan junto con el padre |\n");
    printf("|   Los demonios quedan corriendo           |\n");
    printf("*********************************************\n\n");

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
    while (tipo == 'D' && getppid() != 1)
    {
        // si es un demonio
        // antes de mostrar sus datos
        // esperamos que finalice el proceso padre
    }

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

            // si es zombie, puede tener hijos demonio
            // el primer zombie tiene 2 demonios
            // el segundo zombie tiene 1 demonio
            cantidad_demonios = cantidad_zombies;

            // si es el proceso demonio
            if (crearDemonios() == 0)
            {
                // salimos del while que fue creado en su padre
                break;
            }

            // finalizar proceso para que sea zombie
            exit(0);
        }

        // indicar que el zombie fue creado y crear el siguiente si lo hay
        cantidad_zombies--;
    }
}

int crearDemonios()
{
    int hijo_pid;

    while (cantidad_demonios)
    {
        // crear nuevo demonio
        hijo_pid = fork();

        // si estamos dentro del proceso demonio
        if (hijo_pid == 0)
        {
            // mostrar datos del proceso demonio
            mostrarEntidad('D');

            while (1)
            {
                // mantener proceso demonio vivo
            }

            // salir del while porque fue creado
            // para el proceso padre
            return 0;
        }

        // indicar que el demonio fue creado y crear el siguiente si lo hay
        cantidad_demonios--;
    }

    return getpid();
}