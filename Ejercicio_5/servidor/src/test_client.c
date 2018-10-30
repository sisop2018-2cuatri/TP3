#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>

#define SIZE 100000
#define NAME "shmnotas"

sem_t* mutex_i;
sem_t* mutex_d;
sem_t* mutex_f;

int main(){
	printf("INICIO CLIENTE DE PRUEBA\n");
	int fd = shm_open(NAME, O_RDWR, 0666);
	int opcion;

	mutex_d = sem_open("mutex_d", O_CREAT);
	mutex_i = sem_open("mutex_i", O_CREAT);
	mutex_f = sem_open("mutex_f", O_CREAT);

	if(fd<0){
		perror("shm_open()");
		return EXIT_FAILURE;
	}

	char *comando = (char *)mmap(0, sizeof(char)*6, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	char var[6];
	printf("receiver mapped address: %p\n",comando);

	printf("TEST SHM\n");
	//printf("COMANDO: %s\n",comando);
	
	while(opcion!=1){
		printf("0] Enviar string\n1] Cerrar\n");
		scanf("%d",&opcion);
		switch(opcion){
			case 0:
				printf("INGRESE PALABRA:\n");
				sem_wait(mutex_i);
				printf("PASO POR EL WAIT I\n");
				scanf("%s",comando);
				printf("ANTES DEL STRCPY\n");
				//strcpy(comando,"ret");
				printf("DESPUES DEL STRCPY\n");
				sem_post(mutex_d);
				printf("PASO POR EL POST D\n");
				sem_wait(mutex_f);
				printf("PASO POR EL WAIT F\n");
				//printf("%s",comando);
				sem_post(mutex_i);
				printf("PASO POR EL POST I\n");
				break;
			case 1:
				sem_unlink("mutex_i");
				sem_unlink("mutex_f");
				sem_unlink("mutex_d");
				munmap(comando, sizeof(char)*6);
				return 1;
				break;
		}
	}

	return 1;
}