// C program to illustrate
// pipe system call in C
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define TAM 1024

int main()
{
    int fd;
    FILE *fp;
    char buf[TAM];
    //Creo FIFO en el el writer
    char* myfifo="/tmp/myfifo";
    mkfifo(myfifo, 0666);
    printf("Correr proceso Reader para leer FIFO file \n");
    fd=open(myfifo, O_WRONLY);
    fp = fopen ("file.txt", "r");
    while(1) {
     if(fgets(buf,sizeof(buf),fp)!=NULL)
       if( feof(fp) ) { 
         break ;
      }
	  write(fd, buf, sizeof(buf));
          printf("%s", buf);
   }   
    
    fclose(fp);
    close(fd);
    //unlink(myfifo);
    return 0;
}
