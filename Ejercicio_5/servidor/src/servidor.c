
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

int exit_signal=0;
int fd;

sem_t* mutex_i;
sem_t* mutex_d;
sem_t* mutex_f;


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

    // Iniciar servidor
    inicializar_servidor();
    
    return 1;

	/*char* home_dir = getenv("HOME");
	s_config configuracion;
	int fd;
	char* local_path="../conf/config.conf";
	
	if(load_config(local_path,&configuracion)== READ_FAILURE){
		debug_sys("No hay archivo local, configuracion por defecto");
		configuracion.modo_ejecucion=1;
		configuracion.cantidad_clientes_maxima=10;
		configuracion.mostrar_ayuda=1;
	}	
	mostrar_config(&configuracion);

	fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
	if(fd<0){
		perror("shm_open()");
		return EXIT_FAILURE;
	}

	ftruncate(fd, SIZE);

	int *data = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("sender mapped address: %p\n",data);

	for(int i = 0; i < 4; ++i){
		data[i]=i;
	}
	munmap(data, SIZE);

	close(fd);
	*/
	//return EXIT_SUCCESS;
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
	system("rm /dev/shm/*");
	fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
	/*sem_init(&mutex_d,1,0);
	sem_init(&mutex_i,1,1);
	sem_init(&mutex_f,1,0);
	*/

	printf("Antes de los sem_open\n");
	//recordar limpiar semaforos si es que me dan cualquier cosa
	mutex_d = sem_open("mutex_d", O_CREAT, 0660, 0);
	mutex_i = sem_open("mutex_i", O_CREAT, 0660, 1);
	mutex_f = sem_open("mutex_f", O_CREAT, 0660, 0);

	printf("Despues del open\n");

	if(fd<0){
		perror("shm_open()");
		close(fd);
		shm_unlink(NAME);
		return EXIT_FAILURE;
	}

	printf("antes de truncar\n");
	ftruncate(fd, SIZE);
	printf("Despues de la truncada\n");
	printf("Antes de shm\n");
	char *comando = (char *)mmap(0, sizeof(char)*6, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	//printf("sender mapped address: %p\n",comando);
	//signal(SIGTERM, cerrar_servidor);

	printf("Antes del while\n");
	fflush(stdout);
	while(exit_signal!=1){
		printf("Antes del wait del sem\n");
		sem_wait(mutex_d);
		printf("COMANDO: %s\n",comando);
		sem_post(mutex_f);
	}

	printf("SERVIDOR CERRANDOSE...\n");
	munmap(comando, sizeof(char)*6);
	close(fd);
	shm_unlink(NAME);
	sem_unlink("mutex_i");
	sem_unlink("mutex_f");
	sem_unlink("mutex_d");
}

void cerrar_servidor(){
	exit_signal=1;
}

