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
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

sem_t* mutex_i;
sem_t* mutex_d;
sem_t* mutex_f;
t_mensaje* data;
s_config configuracion;

int main(){
    
    int fd;
    cargar_config(&configuracion);

    if (configuracion.modo_ejecucion == 1){
        printf("**MODO DEBUG ACTIVADO**\n\nSETTINGS:\n");
        mostrar_config(&configuracion); // mostrar valores de configuración
    }

    if (configuracion.mostrar_ayuda == 1){
        printf("\nAYUDA:\n");
        mostrar_ayuda(); // si hay que mostrar la ayuda en el inicio
    }

    // inicializar la conexión
    inicializar_conexion();
    
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
        "reinicie la ejecución del cliente.\n"
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
        printf("\nOpciones: \n");
        printf("1] Cargar nueva nota\n");
        printf("2] Obtener promedio en la materia\n");
        printf("3] Ver promedio general del alumno\n");
        printf("\n");
        printf("4] Salir\n");

        do
        {
            printf("\nIntroduzca una opción (1-4): ");
            fflush(stdin);
            scanf(" %c", &opcion);

            while ((ch = getchar()) != EOF && ch != '\n'){
                // borrar todo lo que no es el primer caracter
                // osea únicamente nos intereza la opción
            }

            if (opcion < '1' || opcion > '4'){
                printf("Opción incorrecta\n");
            }
        } while (opcion < '1' || opcion > '4');

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
        default:
            // fin menú
            break;
        }
        if(opcion != '4') pausa();
    } while (opcion != '4');

    // cerrar la conexión
    finalizar_conexion();
}

//  ====================================
//          FUNCIONES DEL MENÚ
//  ====================================


//ver cliente.h
void cargar_nueva_nota(void)
{
    t_mensaje nota;
    nota.codigo = CARGAR;
    int eva_aux;

    printf("INGRESE DNI:\n");
    scanf("%ld",&(nota.dni));

    printf("TIPO DE EVALUACION:\n1]Primer Parcial\n2]Segundo Parcial\n3]Recuperatorio\n");
    do{       
        scanf("%d",&eva_aux);
        if(eva_aux<1 || eva_aux>3)printf("\nIngrese uno valido entre los nombrados por favor\n");
    }while(eva_aux<1 || eva_aux>3);
    nota.evaluacion = eva_aux;

    printf("INGRESE NOTA\n");
    scanf("%f",&(nota.nota));
    
    strcpy(nota.materia, configuracion.materia);
    
    sem_wait(mutex_i);
    
    *data = nota;

    sem_post(mutex_d);
    sem_wait(mutex_f);

    //leo respuesta
    if(data->codigo == EXITO){
        printf("CARGA EXITOSA\n");
        printf("CODIGO RECIBIDO:%d\n", data->codigo);
    }
    else{
        printf("CARGA FALLIDA\n");
        print_error(data->codigo);
    }
    sem_post(mutex_i);
}


//ver cliente.h
void obtener_promedio_materia(void)
{
    t_mensaje nota;
    nota.codigo = PROMAT;

    printf("INGRESE DNI:\n");
    scanf("%ld",&(nota.dni));
    strcpy(nota.materia, configuracion.materia);
   
    sem_wait(mutex_i);
    
    *data = nota;

    sem_post(mutex_d);
    sem_wait(mutex_f);

    //leo respuesta
    if(data->codigo == EXITO){
        printf("CONSULTA EXITOSA\n");
        printf("PROMEDIO MATERIA %s: %.2f\n", data->materia, data->nota);
    }
    else{
        printf("CONSULTA FALLIDA\n");
    }

    sem_post(mutex_i);
}


//ver cliente.h
void obtener_promedio_general(void)
{
    t_mensaje nota;
    nota.codigo = PROGEN;
    
    //Ingreso de DNI
    printf("INGRESE DNI:\n");
    scanf("%ld",&(nota.dni));
   
    sem_wait(mutex_i);
    
    //Paso la variable local a la shm
    *data = nota;

    sem_post(mutex_d);
    sem_wait(mutex_f);

    //Leo respuesta
    if(data->codigo == EXITO){
        printf("CONSULTA EXITOSA\n");
        printf("PROMEDIO GENERAL: %.2f\n", data->nota);
    }
    else{
        printf("CONSULTA FALLIDA\n");
    }

    sem_post(mutex_i);
}

int inicializar_conexion(){
    int fd = shm_open(NAME, O_RDWR, 0666);
    int opcion;

    mutex_d = sem_open("mutex_d", O_CREAT);
    mutex_i = sem_open("mutex_i", O_CREAT);
    mutex_f = sem_open("mutex_f", O_CREAT);

    if(fd<0){
        perror("shm_open()");
        return EXIT_FAILURE;
    }

    data = (t_mensaje *)mmap(0, sizeof(t_mensaje), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    return 1;
};

void finalizar_conexion(){};

void pausa(){
    while( getchar() != '\n' );
    printf("Presiona [Enter] para continuar: ");
    while( getchar() != '\n' );
}

void print_error(int e){
    switch(e){
        case DNI_NEGATIVO:
            printf("ERROR: DNI NEGATIVO\n");
            break;
        case MATERIA_VACIA:
            printf("ERROR: MATERIA VACIA\n");
            break;
        case MATERIA_LARGA:
            printf("ERROR: MATERIA LARGA\n");
            break;
        case MATERIA_COMA:
            printf("ERROR: MATERIA CON COMA\n");
            break;
        case TIPO_EV_INV:
            printf("ERROR: TIPO DE EVALUACION INVALIDO\n");
            break;
        case NOTA_INVALIDA:
            printf("ERROR: NOTA INVALIDA, NO ESTA ENTRE 1 O 10\n");
            break;
        case FALLA_GUARDADO:
            printf("ERROR: NO SE PUDO GUARDAR REGISTRO\n");
            break;
        case REPETIDO:
            printf("ERROR: REGISTRO REPETIDO\n");
            break;
    }
}