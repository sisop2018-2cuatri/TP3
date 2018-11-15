/*
 *EJERCICIO 5
 *Git: https://github.com/sisop2018-2cuatri/TP3/Ejercicio_5
 *Programa: servidor.c
 *Descripcion: Genera memoria compartida y administra la db segun los mensajes que se pasen por esta
 *Autor: maximiliano-rdl
 *Fecha: 15/11/2018
 **/

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <config_loader.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <servidor.h>
#include <defines.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <time.h>
#include <notas.h>

int fd; //file descriptor usado para la shared memory
t_mensaje *data; //mensaje usado para comunicarse a travez de la shared memory
sem_t* mutex_i, *mutex_d, *mutex_f, *mutex_DB; //semaforos a usar
FILE* db; //base de datos
s_config configuracion; //estructura de configuracion

int main(){
    int fd;

    printf("SERVIDOR DE NOTAS DE ALUMNOS\n");

    cargar_config(&configuracion);

    if (configuracion.modo_ejecucion == 1){
        printf("**MODO DEBUG ACTIVADO**\n\nSETTINGS:\n");
        mostrar_config(&configuracion); // mostrar valores de configuración
    }

    if (configuracion.mostrar_ayuda == 1){
        printf("AYUDA:\n");
        mostrar_ayuda(); // si hay que mostrar la ayuda en el inicio
    }

    if (inicializar_notas(DB_PATH) != 1) //ver si lo agrego al archivo de configuracion
    {
        printf("ERROR: no se pudo cargar el archivo de notas, se modifico la locacion de db\n");
    }

    // Iniciar servidor
    inicializar_servidor();
    
    return 1;
}

void mostrar_ayuda(void)
{
    char *mensaje_ayuda =
        "---------------------------\n"
        "Se puede configurar utilizando el archivo\n"
        "de configuración \"./conf/config.conf\"\n"
        "Para aplicar los cambios guarde el archivo y luego\n"
        "reinicie la ejecución del servidor.\n"
        "----------------------------\n"
        "\n";

    printf("%s", mensaje_ayuda);
}

int inicializar_servidor(){
	fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600); //creacion de memoria compartida
	//recordar limpiar semaforos si es que me dan cualquier cosa
	mutex_d = sem_open("mutex_d", O_CREAT, 0660, 0); //creacion de semaforo, acceso server a shm
	mutex_i = sem_open("mutex_i", O_CREAT, 0660, 1); //creacion de semaforo
	mutex_f = sem_open("mutex_f", O_CREAT, 0660, 0); //creacion de semaforo
	mutex_DB = sem_open("mutex_DB", O_CREAT, 0660, 1); //creacion de semaforo de acceso a la DB
	int db_access, res;
	float prom;

	//stat(DB_PATH, &db_attr); //Consigo los atributos del archivo db para futuro
	
	if(fd<0){
		perror("shm_open()"); //no se puedo abrir la shm, quizas ya hay una creada?
		cerrar_servidor();
		
		return EXIT_FAILURE;
	}

	size_t page_size = 4096; //defino el tamaño de pagina
	
	ftruncate(fd,TAM); //trunco la memoria compartida a el tamaño TAM

	//Mapeo al file descriptor fd una estructura t_mensaje usada para comunicarme entre cliente y servidor
	t_mensaje *data = (t_mensaje *)mmap(0, TAM, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 

	signal(SIGINT, cerrar_servidor); //atrapo la señal sigint y cierro el servidor
	signal(SIGTERM, cerrar_servidor); //atrapo la señal sigterm y cierro el servidor

	while(1){ //se cierra con interrupiones, uma delicia
		printf("\nESPERANDO SOLICITUDES...\n");
		sem_wait(mutex_d);
		printf("\n<<SOLICITUD RECIBIDA>>\n");
		data->evaluacion--;
		printf("--> MENSAJE RECIBIDO:\nCODIGO: %d\tDNI: %ld\tMATERIA: %s\tTIPO EV: %d\nNOTA: %f\n", data->codigo, data->dni, data->materia, data->evaluacion, data->nota);
		sem_wait(mutex_DB);
		res=procesar_solicitud(data);
		printf("<-- MENSAJE A ENVIAR:\nCODIGO: %d\tDNI: %ld\tMATERIA: %s\tTIPO EV: %d\nNOTA: %f\n", data->codigo, data->dni, data->materia, data->evaluacion, data->nota);
		sem_post(mutex_DB);
		data->codigo = res;
		sem_post(mutex_f);
	}
}

void cerrar_servidor(){
	print_debug("\nSERVIDOR CERRANDOSE...\n");
	print_debug("MUNMAP DATA...");
	munmap(data, TAM);
	print_debug("CLOSE FILE...");
	close(fd);
	print_debug("UNLINK SHM...");
	shm_unlink(NAME);
	print_debug("UNLINK SEM I...");
	sem_unlink("mutex_i");
	print_debug("UNLINK SEM F...");
	sem_unlink("mutex_f");
	print_debug("UNLINK SEM D...");
	sem_unlink("mutex_d");
	print_debug("UNLINK SEM DB...");
	sem_unlink("mutex_DB");

	print_debug("FIN\n");
	exit(EXIT_SUCCESS);
}

int abrir_archivo(FILE** fp,const char* path,const char* modo){
	*fp = fopen(path, modo);
	return *fp==NULL?0:1;
}

void cerrar_archivo(FILE** fp){
	printf("INTENTANDO CERRAR ARCHIVO...");
	fclose(*fp);
}

int procesar_solicitud(t_mensaje* m){

	t_registro reg;
	float res;

	switch(m->codigo){
		case CARGAR:
			printf("CARGAR NOTA\n");			
			res = cargar_nota(m->dni, m->materia, m->evaluacion, m->nota);
			if(res==EXITO)
				printf("CARGA EXITOSA!\n");
			else{
				print_error(res);
				return res;
			}
			break;
		case PROMAT:
			printf("PROMEDIO MATERIA\n");
			res = get_promedio(m->dni, m->materia);
			if(res!=0){
				printf("CALCULO DE PROMEDIO DE MATERIA EXITOSO!\n");
				m->nota = res;
			}
			else{
				printf("PROMEDIO NO CALCULABLE\n");
				return res;
			}
			break;
		case PROGEN:
			printf("PROMEDIO GENERAL\n");
			res = get_promedio_general(m->dni);
			if(res!=0){
				printf("CALCULO PROMEDIO GENERAL EXITOSO!\n");
				m->nota = res;
			}
			else{
				printf("PROMEDIO NO CALCULABLE\n");
				return res;
			}			
			break;
		default:
			printf("CODIGO INVALIDO\n");
			return CODIGO_INVALIDO;
	}
	return EXITO;
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

void print_debug(char* cadena){
	if(configuracion.modo_ejecucion == 1){
		printf("DEBUG: %s\n", cadena);
	}
}