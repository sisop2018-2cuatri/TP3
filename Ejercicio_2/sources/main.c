#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>

#define MAX_TAM_EXT ((8))    // tamaño de extension aceptado
#define MAX_CANT_EXT ((512)) // cantidad de extensiones diferentes
#define MAX_OWN_TH ((32))    // cantidad de dueños distintos por hilo
#define NOMBRE_OWN ((256))   // nombre del dueño

typedef struct owner
{
    char nombre[NOMBRE_OWN];
    int cantidad;
} owner_t;

char extenciones[MAX_CANT_EXT][MAX_TAM_EXT];
int hilos_creados = 0; // contador de hilos creados
char path[1024];       // path del directorio para analizar
int total = 0;         // totoal de archivos analizados

const char *get_filename_ext(const char *filename);
void *analizador_extension(void *p_extension);
long tamano_archivo(char *arch);
sem_t mutex;       // semáforo para control de threads
sem_t mutex_datos; // semáforo para control de threads

int main()
{
    DIR *dir;            // puntero al directorio
    int c_buffer;        // para limpiar el flujo de entrada
    struct dirent *arch; // primer archivo
    int buscar_arch;     // buscamos si el directorio tiene algún archivo para analizar
    char extension[MAX_TAM_EXT];
    pthread_t thread_id; // thread para cada extensión

    printf("*********************************************\n");
    printf("|   se analizarán archivos con extensión    |\n");
    printf("*********************************************\n");

    do
    {
        printf("Ingrese path del directorio para analizar: \n");
        fflush(stdin);
        scanf("%s", path);

        while ((c_buffer = getchar()) != '\n' && c_buffer != EOF)
        {
            // limpiar buffer
        }

        // abrir directorio
        dir = opendir(path);

        // el directorio no pudo abrirse
        if (dir == NULL)
        {
            printf("ERROR: no se pudo abrir el directorio\n");
        }
    } while (dir == NULL);

    printf("directorio aceptado\n");

    // leer primer archivo del directorio
    do
    {
        arch = readdir(dir);

        if (arch == NULL)
        {
            // si no hay ningún archivo
            break;
        }
        else if ((strcmp(arch->d_name, ".") == 0) || (strcmp(arch->d_name, "..") == 0))
        {
            // si es el directorio actual o el anterior los ignoramos
            continue;
        }
        else if (get_filename_ext(arch->d_name)[0] == '\0')
        {
            // si el "objecto" no tiene extensión también lo ignoramos
            continue;
        }

        break;
    } while (1);

    if (arch == NULL)
    {
        // no encontramos ningún archivo (con extensión) para analizar
        printf("directorio sin archivos para analizar\n");
        printf("este programa analiza archivos con extensión\n");
    }
    else
    {
        printf("analizando directorio...\n\n");

        // comienza el analisis de archivos (con extensión)
        strcpy(extension, get_filename_ext(arch->d_name));

        // en esta matriz global almacenamos las extensiones que estamos analizando
        strcpy(extenciones[hilos_creados++], extension);

        // cerramos puntero al directorio
        closedir(dir);

        // semáforo para atender las solicitudes de a una por una
        sem_init(&mutex, 0, 1);
        sem_init(&mutex_datos, 0, 1);

        // crear un thread para la extensión del archivo
        pthread_create(&thread_id, NULL, analizador_extension, (void *)extension);

        // esperar a que el hilo finalice para terminar
        pthread_join(thread_id, NULL);
        printf("----------------------------------\n");
        printf("TOTAL [%d] archivo%c analizado%c\n",
               total,
               total != 1 ? 's' : '\0',
               total != 1 ? 's' : '\0');

        // cerrar semáforo de threads
        sem_destroy(&mutex);
        sem_destroy(&mutex_datos);
    }

    printf("\n");

    return 0;
}

const char *get_filename_ext(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "\0";
    return dot + 1;
}

long tamano_archivo(char *arch)
{
    FILE *fich;
    long ftam = -1;

    fich = fopen(arch, "r");
    if (fich)
    {
        fseek(fich, 0L, SEEK_END);
        ftam = ftell(fich);
        fclose(fich);
    }
    else
        printf("ERROR: al analizar [%s]\n", arch);

    return ftam;
}

void *analizador_extension(void *p_extension)
{
    DIR *dir;                    // puntero al directorio
    char *extension;             // extensión para analizar en este hilo
    struct dirent *arch;         // primer archivo
    char ext_leida[MAX_TAM_EXT]; // extensión del archivo leido
    int i, temp;
    int flag_hilo_creado;
    pthread_t threads_id[MAX_CANT_EXT - 1]; // thread para cada extensión
    int threads_creados = 0;                // threads hijos de este
    char *nombre_archivo;
    long int tam_archivo;
    struct stat info_archivo;
    struct passwd *pInfo;
    owner_t owners[MAX_OWN_TH]; // dueños de archivos
    int cant_owners = 0;        // cantidad de dueños de archivos diferentes
    int flag_owner_encontrado;
    int cantidad_archivos_ext = 0;
    long int tam_arch_minimo = 0;
    long int tam_arch_maximo = 0;

    // oweners de la mayor cantidad
    owner_t owners_max[MAX_OWN_TH];
    int cant_owners_max = 0; // cantidad de owners dueños de la mayor cantidad
    int owner_max_cant = 0;

    extension = (char *)p_extension;

    dir = opendir(path);
    if (dir != NULL)
    {
        while ((arch = readdir(dir)) != NULL)
        {
            if ((strcmp(arch->d_name, ".") != 0) && (strcmp(arch->d_name, "..") != 0))
            {
                // verificar que el archivo sea de la extensión
                // que estamos analizando en este hilo
                ext_leida[0] != '\0';
                strcpy(ext_leida, get_filename_ext(arch->d_name));
                if (strcasecmp(ext_leida, extension) == 0)
                {
                    // agregar al total de archivos
                    total++;

                    // nuevo archivo de la extensión
                    cantidad_archivos_ext++;

                    // obtener path completo del archivo que estamos analisando
                    temp = strlen(path);
                    nombre_archivo = malloc(temp + strlen(arch->d_name) + 2);
                    if (path[temp - 1] == '/')
                        sprintf(nombre_archivo, "%s%s", path, arch->d_name);
                    else
                        sprintf(nombre_archivo, "%s/%s", path, arch->d_name);

                    // obtener información del archivo
                    stat(nombre_archivo, &info_archivo);
                    pInfo = getpwuid(info_archivo.st_uid);

                    // recorrer la matriz de owners y agregarlo si no está
                    flag_owner_encontrado = 0;
                    for (i = 0; i < cant_owners; i++)
                    {
                        if (strcmp(owners[i].nombre, pInfo->pw_name) == 0)
                        {
                            flag_owner_encontrado = 1;
                            owners[i].cantidad++;
                            break;
                        }
                    }

                    // si encontramos un nuevo dueño de archivos
                    if (!flag_owner_encontrado)
                    {
                        // indicar que es dueño del archivo
                        strcpy(owners[cant_owners].nombre, pInfo->pw_name);
                        owners[cant_owners++].cantidad = 1;
                    }

                    // peso del más y del menos pesado
                    tam_archivo = tamano_archivo(nombre_archivo);
                    if (tam_archivo >= 0)
                    {
                        if (tam_archivo > tam_arch_maximo || tam_arch_maximo == 0)
                        {
                            tam_arch_maximo = tam_archivo;
                        }

                        if (tam_archivo < tam_arch_minimo || tam_arch_minimo == 0)
                        {
                            tam_arch_minimo = tam_archivo;
                        }
                    }
                }
                else if (ext_leida[0] != '\0' && ext_leida[0] != '\n')
                {
                    // los threads deben verificar de a uno
                    sem_wait(&mutex);
                    // si no es una extensión para analizar en este hilo
                    // verificar si no se esta analizando en ningún hilo
                    flag_hilo_creado = 0;
                    for (i = 0; i < 512 && i < hilos_creados; i++)
                    {
                        if (strcasecmp(extenciones[i], ext_leida) == 0)
                        {
                            flag_hilo_creado = 1;
                        }
                    }

                    // si no se está analizando en ningún hilo
                    // creamos un nuevo hilo que analiza este tipo de extensión
                    if (i == hilos_creados && !flag_hilo_creado)
                    {
                        strcpy(extenciones[hilos_creados++], ext_leida);
                        pthread_create(&threads_id[threads_creados++], NULL, analizador_extension, (void *)extenciones[hilos_creados - 1]);
                    }

                    // fin verificación
                    sem_post(&mutex);
                }
            }
        }

        // cerrar directorio
        closedir(dir);
    }
    else
    {
        printf("ERROR: abriendo directorio [%s] desde thread\n", path);
    }

    // esperamos que finalice cada thread hijo de este
    for (i = 0; i < threads_creados; i++)
    {
        pthread_join(threads_id[i], NULL);
    }

    // los threads deben mostrar los datos de a uno
    sem_wait(&mutex_datos);

    printf("----------------------------------\n");
    printf("extensión [%s]\n", extension);
    printf("cantidad archivos [%d]\n", cantidad_archivos_ext);
    printf("peso del más pesado [%ld] bytes\n", tam_arch_maximo);
    printf("peso del menos pesado [%ld] bytes\n", tam_arch_minimo);

    // mostrar el owner de la mayor cantidad
    for (i = 0; i < cant_owners; i++)
    {
        if (owners[i].cantidad > owner_max_cant || owner_max_cant == 0)
        {
            // si hay un nuevo mayor dueño de más archivos
            owner_max_cant = owners[i].cantidad;
            cant_owners_max = 1;
            strcpy(owners_max[cant_owners_max - 1].nombre, owners[i].nombre);
        }
        else if (owners[i].cantidad == owner_max_cant)
        {
            // si hay otro owner con la mayor cantidad
            cant_owners_max++;
            strcpy(owners_max[cant_owners_max - 1].nombre, owners[i].nombre);
        }
    }

    if (cant_owners_max == 1)
    {
        printf("dueño de la mayor cantidad [%s]\n", owners_max[0].nombre);
    }
    else
    {
        printf("dueños de la mayor cantidad:\n");
        for (i = 0; i < cant_owners_max; i++)
        {
            printf(" - [%s]\n", owners_max[i].nombre);
        }
    }

    if (cant_owners_max == 1)
    {
        printf("es dueño");
    }
    else
    {
        printf("son dueños");
    }

    printf(" de [%d] archivo%c\n", owner_max_cant, owner_max_cant != 1 ? 's' : '\0');

    // fin msotrar datos
    sem_post(&mutex_datos);
}