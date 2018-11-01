
/*
 *EJERCICIO 5
 *Git: https://github.com/sisop2018-2cuatri/TP3/Ejercicio_5
 *Programa: servidor.c
 *Descripcion: Genera memoria compartida y administra la db segun los mensajes que se pasen por esta
 *Autor: maximiliano-rdl
 *Fecha: 24/11/2018
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

int exit_signal=0, fd;
t_mensaje *data;
sem_t* mutex_i, *mutex_d, *mutex_f, *mutex_DB;
FILE* db;
struct stat db_attr;

int main(){
	
	s_config configuracion;
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
        "SERVIDOR DE NOTAS DE ALUMNOS\n"
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
	//system("rm /dev/shm/*"); //MOMENTANEO; CAMBIAR A ALGO MEJOR
	fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
	//recordar limpiar semaforos si es que me dan cualquier cosa
	mutex_d = sem_open("mutex_d", O_CREAT, 0660, 0);
	mutex_i = sem_open("mutex_i", O_CREAT, 0660, 1);
	mutex_f = sem_open("mutex_f", O_CREAT, 0660, 0);
	mutex_DB = sem_open("mutex_DB", O_CREAT, 0660, 1);
	int db_access, res;
	float prom;

	stat(DB_PATH, &db_attr); //Consigo los atributos del archivo db para futuro

	/*
	printf("ESPERANDO ACCESSO AL ARCHIVO\n");
	sem_wait(mutex_DB);
	abrir_archivo(&db, DB_PATH, "r+");
	*/
	
	if(fd<0){
		perror("shm_open()");
		close(fd);
		//shm_unlink(NAME);
		cerrar_servidor();
		return EXIT_FAILURE;
	}

	size_t page_size = 4096;
	
	printf("CALCULO TAMAÑO\n");
	//size_t tam = sizeof(t_mensaje);
	
	ftruncate(fd,TAM);

	t_mensaje *data = (t_mensaje *)mmap(0, TAM, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	signal(SIGINT, cerrar_servidor);
	signal(SIGTERM, cerrar_servidor);

	while(exit_signal!=1){
		printf("ESPERANDO SOLICITUDES\n");
		sem_wait(mutex_d);
		printf("SOLICITUD RECIBIDA\n");
		data->evaluacion--;
		printf("CODIGO: %d\tDNI: %ld\tMATERIA: %s\tTIPO EV: %d\nNOTA: %f\n", data->codigo, data->dni, data->materia, data->evaluacion, data->nota);
		sem_wait(mutex_DB);
		res=procesar_solicitud(data);
		printf("CODIGO: %d\tDNI: %ld\tMATERIA: %s\tTIPO EV: %d\nNOTA: %f\n", data->codigo, data->dni, data->materia, data->evaluacion, data->nota);
		sem_post(mutex_DB);
		data->codigo = res;
		sem_post(mutex_f);
	}

	printf("\nSERVIDOR CERRANDOSE...\n");
	munmap(data, TAM);
	close(fd);
	shm_unlink(NAME);
	sem_unlink("mutex_i");
	sem_unlink("mutex_f");
	sem_unlink("mutex_d");
	sem_unlink("muted_DB");
	cerrar_archivo(&db);
}

void cerrar_servidor(){
	exit_signal=1;
	printf("\nSERVIDOR CERRANDOSE...\n");
	printf("***MUNMAP DATA**\n");
	munmap(data, TAM);
	printf("***CLOSE FILE***\n");
	close(fd);
	printf("***UNLINK SHM***\n");
	shm_unlink(NAME);
	printf("***UNLINK SEM I***\n");
	sem_unlink("mutex_i");
	printf("***UNLINK SEM f***\n");
	sem_unlink("mutex_f");
	printf("***UNLINK SEM d***\n");
	sem_unlink("mutex_d");
	printf("***UNLINK SEM DB***\n");
	sem_unlink("mutex_DB");
	printf("***CERRAR DB***\n");
	cerrar_archivo(&db);

	exit(EXIT_SUCCESS);
}

int abrir_archivo(FILE** fp,const char* path,const char* modo){
	*fp = fopen(path, modo);
	return *fp==NULL?0:1;
}

void cerrar_archivo(FILE** fp){
	fclose(*fp);
}

int procesar_solicitud(t_mensaje* m){

	//TODO: fijarme la fecha del archivo, si coincide con la fecha cargada, proceso
	//si no coincide, cargo archivo de nuevo y trabajo con el actualizado.
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
				return FALLA;
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
				printf("PROMEDIO NO CALCULABLE");
				return FALLA;
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
				printf("PROMEDIO NO CALCULABLE");
				return FALLA;
			}			
			break;
		default:
			printf("CODIGO INVALIDO\n");
			return CODIGO_INVALIDO;
	}
	return EXITO;
}

float prom_mat(int dni, char* materia){

}

float prom_gen(int dni){

}

/*
int cargar_nota(t_mensaje* m){
    struct stat db_attr_act;
    stat(DB_PATH, &db_attr_act);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
    if(db_attr_act.st_mtime != db_attr.st_mtime){
    	db_attr=db_attr_act;
    }
    return 1;
}*/

/*#define DNI_NEGATIVO 2
#define MATERIA_VACIA 3
#define MATERIA_LARGA 4
#define MATERIA_COMA 5
#define TIPO_EV_INV 6
#define NOTA_INVALIDA 7
#define EXITO 1
#define FALLA_GUARDADO 9
#define REPETIDO 8*/

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