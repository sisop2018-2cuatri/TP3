









all: servidor

install: 
	$(shell ./.install.sh)

servidor: servidor.o protocol.o string_utils.o
	gcc -Iinclude obj/servidor.o obj/protocol.o obj/string_utils.o -o bin/servidor

servidor.o: src/servidor/servidor.c
	gcc -Iinclude -c src/servidor/servidor.c -o obj/$@

protocol.o: src/misc/protocol.c
	gcc -Iinclude -c src/misc/protocol.c -o obj/$@

string_utils.o: src/misc/string_utils.c
	gcc -Iinclude -c src/misc/string_utils.c -o obj/$@

clean:
	rm obj/*.o; rm bin/*
