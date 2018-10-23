run: ./objects/cliente.o ./objects/configuracion.o ./objects/conexion.o
	gcc ./objects/cliente.o ./objects/configuracion.o ./objects/conexion.o -o ./run

./objects/conexion.o: ./sources/conexion.c 
	gcc -c ./sources/conexion.c -o ./objects/conexion.o

./objects/configuracion.o: ./sources/configuracion.c 
	gcc -c ./sources/configuracion.c -o ./objects/configuracion.o

./objects/cliente.o: ./sources/cliente.c
	gcc -c ./sources/cliente.c -o ./objects/cliente.o