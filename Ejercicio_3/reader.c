// C program to illustrate
// pipe system call in C
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_BUF 1024
int main(int argc, char *argv[])
{
    int fd,log,fin,fac;
    char buf[MAX_BUF];
    char bus[MAX_BUF];
    time_t t,t1;
    struct tm *tm,*tp;
    char fecha[100];
    char arcfecha[100];
    char nombre[20] = "Movimientos_";
    char pagos[25] = "Facturacion_";
    FILE *fp;

    t=time(NULL);
    t1=time(NULL);
    tm=localtime(&t);
    tp=localtime(&t1);
    strftime(fecha,100," %Y%m%d",tm);
    strftime(arcfecha,100," %Y%m%d",tp);
    strcat(nombre,arcfecha);
    strcat(pagos,arcfecha);
 
    //Creo FIFO en el el writer
    char* myfifo="/tmp/myfifo";
    printf("Abrir , leer y mostrar mensaje \n");
    fd=open(myfifo, O_RDWR);
    
      	while(buf!=NULL) {

        read(fd, buf, sizeof(buf));
        //Comienzo del archivo de movimientos

        log=open(nombre, O_WRONLY | O_APPEND | O_CREAT, 0777);
	
	if(buf[0]=='E')
	{
	  printf("Entrada %s \n",buf);
          char entrada[20] = "Entrada ";
	  //printf("Entrada %s %s \n",buf,fecha);
          strcat(entrada,buf);
	  write(log,entrada,strlen(entrada));

	}
    if(buf[0]=='S')
    {
    char *precio=argv[1];
    printf("Salida %s \n",buf);
    //printf("%s \n",other);
    char salida[20] = "Salida ";
    strcat(salida,buf);
    write(log,salida,strlen(salida));

    }
     if(buf[0]!='E' && buf[0]!='S')
       {
	exit(0);
       } 
    }    
    //Comienzo del archivo de facturacion
    printf("%s \n",nombre);
    fac=open(nombre, O_RDWR);
    if(bus!=NULL)
	{
    read(fac, bus, sizeof(bus));
    
    fin=open(pagos, O_WRONLY | O_APPEND | O_CREAT, 0777);
    
    	if(bus[0]=='E')
	{
	  printf("%s \n",bus);
          //strcat(entrada,buf);
	  write(fin,bus,strlen(bus));

	}
    if(bus[0]=='S')
    {
          //printf("%s \n",bus);
          //strcat(entrada,buf);
	  write(fin,bus,strlen(bus));

    }
     if(bus[0]!='E' && bus[0]!='S')
       {
	exit(0);
       }
     }
   
   close(fd);
  
    return 0;
}
