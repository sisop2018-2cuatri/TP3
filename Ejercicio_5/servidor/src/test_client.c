#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>

#define SIZE 100000
#define NAME "shmnotas"

typedef struct s_mensaje{
	char codigo[6];
	long int dni;
	char materia[30];
	float nota;
}t_mensaje;

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
	t_mensaje nota;
	t_mensaje *data = (t_mensaje *)mmap(0, sizeof(t_mensaje), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	while(opcion!=1){
		printf("0] Enviar NOTA\n1] Cerrar\n");
		scanf("%d",&opcion);
		switch(opcion){
			case 0:
				//Preparo mensaje a mandar
				strcpy(nota.codigo,"CARGAR");
				printf("INGRESE DNI:\n");
				scanf("%ld",&(nota.dni));
				printf("INGRESE NOTA\n");
				scanf("%f",&(nota.nota));
				strcpy(nota.materia, "SISTEMAS OPERATIVOS");
				
				sem_wait(mutex_i);
				*data = nota;
				sem_post(mutex_d);
				sem_wait(mutex_f);
				
				//leo respuesta
				if(strcmp(data->codigo,"EXITO")==0){
					printf("CARGA EXITOSA\n");
					printf("CODIGO RECIBIDO:%s\n", data->codigo);
				}
				else{
					printf("CARGA FALLIDA\n");
					printf("CODIGO RECIBIDO:%s\n", data->codigo);
				}
				sem_post(mutex_i);
				break;
			case 1:
				sem_unlink("mutex_i");
				sem_unlink("mutex_f");
				sem_unlink("mutex_d");
				munmap(data, sizeof(char)*6);
				return 1;
				break;
		}
	}
	return 1;
}