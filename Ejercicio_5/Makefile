C_INC = -Iinclude
R_SRV = src/servidor
R_MIS = src/misc
R_CLN = src/cliente



all: servidor cliente

install: 
	$(shell ./.install.sh)

servidor: servidor.o protocol.o string_utils.o
	gcc $(C_INC) obj/servidor.o obj/protocol.o obj/string_utils.o -o bin/servidor -lrt

cliente: cliente.o protocol.o string_utils.o
	gcc $(C_INC) obj/cliente.o obj/protocol.o obj/string_utils.o -o bin/cliente -lrt

servidor.o: src/servidor/servidor.c
	gcc $(C_INC) -c $(R_SRV)/servidor.c -o obj/$@ 

cliente.o: src/cliente/cliente.c
	gcc $(C_INC) -c $(R_CLN)/cliente.c -o obj/$@

protocol.o: src/misc/protocol.c
	gcc $(C_INC) -c $(R_MIS)/protocol.c -o obj/$@

string_utils.o: src/misc/string_utils.c
	gcc $(C_INC) -c $(R_MIS)/string_utils.c -o obj/$@

clean:
	rm obj/*.o; rm bin/*
