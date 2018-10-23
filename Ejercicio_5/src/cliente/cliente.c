#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <protocol.h>
#include <string.h>

int main(){
	s_config configuracion;
	char* home= getenv("HOME");
	load_config(strcat(home,"/.config/adlms/Ejercicio5/conf.conf"),&configuracion);
	printf("%s",configuracion.name);
	int fd = shm_open(configuracion.name, O_RDONLY, 0666);
	if(fd < 0){
		perror("shm_open");
		return EXIT_FAILURE;
	}	

	int *data = (int *)mmap(0, configuracion.size, PROT_READ, MAP_SHARED, fd, 0);
	printf("receiver mapped address: %p\n",data);

	for(int i = 0; i < 4; ++i){
		printf("%d\n", data[i]);
	}

	munmap(data, configuracion.size);

	close(fd);

	shm_unlink(configuracion.name);

	return EXIT_SUCCESS;
}
