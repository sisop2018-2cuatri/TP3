C_INC = -Iinclude
R_SRV = src
R_UTL = src/utils



all: servidor

install: 
	$(shell ./install.sh)

servidor: servidor.o protocol.o string_utils.o
	gcc $(C_INC) obj/servidor.o obj/protocol.o obj/string_utils.o -o bin/servidor -lrt

servidor.o: src/servidor.c
	gcc $(C_INC) -c $(R_SRV)/servidor.c -o obj/$@ 

notas.o: $(R_SRV)/notas.c
	gcc $(C_INC) -c $(R_SRV)/notas.c -o obj/$@

protocol.o: $(R_SRV)/protocol.c
	gcc $(C_INC) -c $(R_SRV)/protocol.c -o obj/$@

string_utils.o: $(R_UTL)/string_utils.c
	gcc $(C_INC) -c $(R_UTL)/string_utils.c -o obj/$@

clean:
	rm obj/*.o; rm bin/*
