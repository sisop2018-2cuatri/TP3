#include "protocol.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>

void main(){
	s_config config;
	
	if(load_config("../conf/conf.conf",&config)!=READ_FAILURE)
		printf("EXITO LECTURA\n");
	else
		printf("ERROR LECTURA\n");
	printf("NAME:%s\nSIZE:%d\n",config.name, config.size);
}
