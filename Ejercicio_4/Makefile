cli_obj = ./cliente/objects/
cli_src = ./cliente/sources/
cli_obj_list = \
	$(cli_obj)cliente.o \
	$(cli_obj)configuracion.o \
	$(cli_obj)conexion.o
srv_obj = ./servidor/objects/
srv_src = ./servidor/sources/
srv_obj_list = \
	$(srv_obj)servidor.o \
	$(srv_obj)conexion.o \
	$(srv_obj)configuracion.o \
	$(srv_obj)notas.o

all: ./cliente/cliente ./servidor/servidor

./servidor/servidor: $(srv_obj_list)
	gcc $(srv_obj_list) -o ./servidor/servidor -lpthread 

$(srv_obj)conexion.o: $(srv_src)conexion.c
	gcc -c $(srv_src)conexion.c -o $(srv_obj)conexion.o

$(srv_obj)configuracion.o: $(srv_src)configuracion.c 
	gcc -c $(srv_src)configuracion.c -o $(srv_obj)configuracion.o

$(srv_obj)notas.o: $(srv_src)notas.c
	gcc -c $(srv_src)notas.c -o $(srv_obj)notas.o

$(srv_obj)servidor.o: $(srv_src)servidor.c
	gcc -c $(srv_src)servidor.c -o $(srv_obj)servidor.o

./cliente/cliente: $(cli_obj_list)
	gcc $(cli_obj_list) -o ./cliente/cliente

$(cli_obj)conexion.o: $(cli_src)conexion.c 
	gcc -c $(cli_src)conexion.c -o $(cli_obj)conexion.o

$(cli_obj)configuracion.o: $(cli_src)configuracion.c 
	gcc -c $(cli_src)configuracion.c -o $(cli_obj)configuracion.o

$(cli_obj)cliente.o: $(cli_src)cliente.c
	gcc -c $(cli_src)cliente.c -o $(cli_obj)cliente.o

clean:
	-rm $(cli_obj)*.o
	-rm $(srv_obj)*.o