run: ./objects/servidor.o ./objects/conexion.o ./objects/configuracion.o ./objects/notas.o
	gcc ./objects/servidor.o ./objects/conexion.o ./objects/configuracion.o ./objects/notas.o -o ./run 

./objects/conexion.o: ./sources/conexion.c 
	gcc -c ./sources/conexion.c -o ./objects/conexion.o

./objects/configuracion.o: ./sources/configuracion.c 
	gcc -c ./sources/configuracion.c -o ./objects/configuracion.o

./objects/notas.o: ./sources/notas.c
	gcc -c ./sources/notas.c -o ./objects/notas.o

./objects/servidor.o: ./sources/servidor.c
	gcc -c ./sources/servidor.c -o ./objects/servidor.o