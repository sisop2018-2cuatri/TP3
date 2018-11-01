C_INC = -Iinclude
R_CLN = src
R_UTL = src/utils



all: cliente

install: 
	$(shell ./install.sh)

cliente: cliente.o config_loader.o print_utils.o string_utils.o
	gcc $(C_INC) obj/cliente.o obj/string_utils.o obj/config_loader.o obj/print_utils.o -o bin/cliente -lrt -lpthread

cliente.o: src/cliente.c
	gcc $(C_INC) -c $(R_CLN)/cliente.c -o obj/$@ 

notas.o: $(R_CLN)/notas.c
	gcc $(C_INC) -c $(R_CLN)/notas.c -o obj/$@

config_loader.o: $(R_CLN)/config_loader.c
	gcc $(C_INC) -c $(R_CLN)/config_loader.c -o obj/$@

string_utils.o: $(R_UTL)/string_utils.c
	gcc $(C_INC) -c $(R_UTL)/string_utils.c -o obj/$@

print_utils.o: $(R_UTL)/print_utils.c
	gcc $(C_INC) -c $(R_UTL)/print_utils.c -o obj/$@

clean:
	rm obj/*.o; rm bin/*
