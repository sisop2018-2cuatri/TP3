all: ./cliente/cliente ./servidor/servidor

./cliente/cliente: ./cliente/objects/cliente.o ./cliente/objects/configuracion.o ./cliente/objects/conexion.o
	gcc ./cliente/objects/cliente.o ./cliente/objects/configuracion.o ./cliente/objects/conexion.o -o ./cliente/cliente

./servidor/servidor: ./servidor/objects/servidor.o ./servidor/objects/conexion.o ./servidor/objects/configuracion.o ./servidor/objects/notas.o
	gcc ./servidor/objects/servidor.o ./servidor/objects/conexion.o ./servidor/objects/configuracion.o ./servidor/objects/notas.o -o ./servidor/servidor -lpthread 

./servidor/objects/conexion.o: ./servidor/sources/conexion.c
	gcc -c ./servidor/sources/conexion.c -o ./servidor/objects/conexion.o

./servidor/objects/configuracion.o: ./servidor/sources/configuracion.c 
	gcc -c ./servidor/sources/configuracion.c -o ./servidor/objects/configuracion.o

./servidor/objects/notas.o: ./servidor/sources/notas.c
	gcc -c ./servidor/sources/notas.c -o ./servidor/objects/notas.o

./servidor/objects/servidor.o: ./servidor/sources/servidor.c
	gcc -c ./servidor/sources/servidor.c -o ./servidor/objects/servidor.o

./cliente/objects/conexion.o: ./cliente/sources/conexion.c 
	gcc -c ./cliente/sources/conexion.c -o ./cliente/objects/conexion.o

./cliente/objects/configuracion.o: ./cliente/sources/configuracion.c 
	gcc -c ./cliente/sources/configuracion.c -o ./cliente/objects/configuracion.o

./cliente/objects/cliente.o: ./cliente/sources/cliente.c
	gcc -c ./cliente/sources/cliente.c -o ./cliente/objects/cliente.o