C_INC = -Iinclude
R_SRV = src
R_UTL = src/utils



all: servidor

install: 
	$(shell ./install.sh)

servidor: servidor.o config_loader.o string_utils.o print_utils.o notas.o
	gcc -g $(C_INC) obj/servidor.o obj/config_loader.o obj/string_utils.o obj/print_utils.o obj/notas.o -o bin/servidor -lrt -lpthread

servidor.o: src/servidor.c
	gcc -g $(C_INC) -c $(R_SRV)/servidor.c -o obj/$@ 

notas.o: src/notas.c
	gcc -g $(C_INC) -c $(R_SRV)/notas.c -o obj/$@ 

notas.o: $(R_SRV)/notas.c
	gcc -g $(C_INC) -c $(R_SRV)/notas.c -o obj/$@

config_loader.o: $(R_SRV)/config_loader.c
	gcc -g $(C_INC) -c $(R_SRV)/config_loader.c -o obj/$@

string_utils.o: $(R_UTL)/string_utils.c
	gcc -g $(C_INC) -c $(R_UTL)/string_utils.c -o obj/$@

print_utils.o: $(R_UTL)/print_utils.c
	gcc -g $(C_INC) -c $(R_UTL)/print_utils.c -o obj/$@

clean:
	rm obj/*.o; rm bin/*
