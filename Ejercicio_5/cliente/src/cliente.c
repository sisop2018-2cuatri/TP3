/*
 *EJERCICIO 5
 *Git: https://github.com/sisop2018-2cuatri/TP3/Ejercicio_5
 *Programa: cliente.c
 *Descripcion: Cliente que permite conectar a un profesor al servidor de notas *universitario
 *Autor: maximiliano-rdl, LautaroLorenz
 *Fecha: 20/11/2018
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
#include <time.h>

sem_t* mutex_i;
sem_t* mutex_d;
sem_t* mutex_f;
t_mensaje* data;
t_mensaje nota;
s_config configuracion;
struct timespec tiempo;
int status_server;
int fd; //file descriptor de la shared memory


int main(){
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
   
   if(inicializar_conexion()==EXIT_FAILURE){
   	printf("Error con la comunicacion con el servidor\nCerrando...\n");
	return 0;
   }
    
    // menú de opciones
    mostrar_menu();
    
    return EXIT_SUCCESS;
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

	printf("Materia: %s\n",configuracion.materia);

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
    } while (opcion != '4' && status_server==1);

    // cerrar la conexión
    finalizar_conexion();
}

//  ====================================
//          FUNCIONES DEL MENÚ
//  ====================================


//ver cliente.h
void cargar_nueva_nota(void)
{
    nota.codigo = CARGAR;
    int eva_aux;

    print_debug("CARGA NOTA");

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
    
    if(peticion_servidor()==EXIT_SUCCESS){
        print_debug("TRANSACCION EXITOSA");
    }else{
        print_debug("TRANSACCION FALLIDA, NO SE PUDO TERMINAR LA TRANSACCION.");
        return;
    }

    //leo respuesta
    if(data->codigo == EXITO){
        printf("CARGA EXITOSA\n");
    }
    else{
        print_error(data->codigo);
    }

    print_debug("LIBERANDO SEMAFORO - PARA PERMITIR COMENZAR NUEVA TRANSACCION");
    sem_post(mutex_i);
    print_debug("SEMAFORO LIBERADO - PUEDE COMENZAR UNA NUEVA TRANSACCION");
}


//ver cliente.h
void obtener_promedio_materia(void)
{
    nota.codigo = PROMAT;

    printf("INGRESE DNI:\n");
    scanf("%ld",&(nota.dni));
    strcpy(nota.materia, configuracion.materia);
   
    if(peticion_servidor()==EXIT_SUCCESS){
        print_debug("TRANSACCION EXITOSA");
    }else{
        print_debug("TRANSACCION FALLIDA, NO SE PUDO TERMINAR LA TRANSACCION.");
        return;
    }

    //leo respuesta
    if(data->codigo == EXITO){
        printf("PROMEDIO MATERIA %s: %.2f\n", data->materia, data->nota);
    }
    else{
        print_error(data->codigo);
    }

    print_debug("LIBERANDO SEMAFORO - PARA PERMITIR COMENZAR NUEVA TRANSACCION");
    sem_post(mutex_i);
    print_debug("SEMAFORO LIBERADO - PUEDE COMENZAR UNA NUEVA TRANSACCION");
}


//ver cliente.h
void obtener_promedio_general(void)
{
    nota.codigo = PROGEN;
    
    //Ingreso de DNI
    printf("INGRESE DNI:\n");
    scanf("%ld",&(nota.dni));
   
    if(peticion_servidor()==EXIT_SUCCESS){
        print_debug("TRANSACCION EXITOSA");
    }else{
        print_debug("TRANSACCION FALLIDA, NO SE PUDO TERMINAR LA TRANSACCION.");
        return;
    }

    //Leo respuesta
    if(data->codigo == EXITO){
        printf("PROMEDIO GENERAL: %.2f\n", data->nota);
    }
    else{
        print_error(data->codigo);
    }

    print_debug("LIBERANDO SEMAFORO - PARA PERMITIR COMENZAR NUEVA TRANSACCION");
    sem_post(mutex_i);
    print_debug("SEMAFORO LIBERADO - PUEDE COMENZAR UNA NUEVA TRANSACCION");
}

int inicializar_conexion(){
    print_debug("ABRO SHARED MEMORY");
    fd = shm_open(NAME, O_RDWR, 0777);
    if(fd<0)
	    print_debug("EROR APERTURA SHM");
    else
	    print_debug("EXITO APERTURA SHM");
    status_server=1;
    print_debug("ABRO SEMAFORO DE LECTURA DE MENSAJE EN SERVIDOR");
    mutex_d = sem_open("mutex_d", O_RDWR);
    print_debug("ABRO SEMAFORO DE TRANSACCION");
    mutex_i = sem_open("mutex_i", O_RDWR);
    print_debug("ABRO SEMAFORO DE LECTURA DE RESPUESTA");
    mutex_f = sem_open("mutex_f", O_RDWR);

    if(fd<0){
        if(configuracion.modo_ejecucion == 1)
		perror("shm_open()");
        return EXIT_FAILURE;
    }

    data = (t_mensaje *)mmap(0, sizeof(t_mensaje), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    tiempo.tv_sec = 3;
    tiempo.tv_nsec = 0;

    return EXIT_SUCCESS;
};

void finalizar_conexion(){
	sem_close(mutex_d);
	sem_close(mutex_i);
	sem_close(mutex_f);
	close(fd);
}

void pausa(){
    while( getchar() != '\n' );
    printf("\nPresiona [Enter] para continuar: \n");
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
            printf("ERROR: MATERIA CON COMA, POR FAVOR SAQUESELA\n");
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
            printf("ERROR: REGISTRO REPETIDO, NO PUEDE PISARSE UNA NOTA YA CARGADA\n");
            break;
        case NOTAS_INSUF:
            printf("ERROR: CANTIDAD DE NOTAS INSUFICIENTES PARA EL ALUMNO\n"
                   "NO TIENE NINGUNA NOTA O LE FALTA RENDIR EL SEGUNDO PARCIAL\n");
            break;
    }
}


void print_debug(char* cadena){
    if(configuracion.modo_ejecucion == 1){
        printf("DEBUG: %s\n", cadena);
    }
}

int peticion_servidor(){
    int res_timedwait; //donde guardo el resultado de la espera

    print_debug("ESPERANDO SEMAFORO - ESPERANDO PERMISO PARA COMENZAR TRANSACCION");
    
    get_tiempo(3);
    res_timedwait = sem_timedwait(mutex_i, &tiempo);
    if(res_timedwait == -1){
        printf("Se acabo el tiempo de espera al servidor.\nServidor offline o en mantenimiento.\nSaliendo...\n");
        status_server=0;
        return EXIT_FAILURE;
    }
    
    print_debug("SEMAFORO PASADO - COMIENZO TRANSACCION");

    print_debug("ESCRIBIENDO EN SHM PETICION");
    *data = nota;

    print_debug("LIBERANDO SEMAFORO - PARA QUE EL SERVIDOR PUEDA LEER PETICION");
    sem_post(mutex_d);
    print_debug("SEMAFORO LIBERADO - AHORA EL SERVIDOR PUEDE LEER PETICION");

    print_debug("ESPERANDO SEMAFORO - ESPERANDO QUE EL SERVIDOR ESCRIBA RESPUESTA");
    
    get_tiempo(3);
    res_timedwait=sem_timedwait(mutex_f, &tiempo);
    if(res_timedwait == -1){
        printf("Se acabo el tiempo de espera al servidor.\nServidor offline o en mantenimiento.\nSaliendo...\n");
        status_server=0;
        return EXIT_FAILURE;
    }
    
    print_debug("SEMAFORO PASADO - LEYENDO RESPUESTA DEL SERVIDOR");

    return EXIT_SUCCESS;
}

void get_tiempo(int timeout){
    clock_gettime(CLOCK_REALTIME, &tiempo);
    tiempo.tv_sec+=timeout;
}