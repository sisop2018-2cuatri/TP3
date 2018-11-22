./run: ./objects/main.o ./objects/cola.o ./objects/lista.o
	gcc ./objects/main.o ./objects/cola.o ./objects/lista.o -o ./run -lpthread

./objects/main.o: ./sources/main.c
	gcc -c ./sources/main.c -o ./objects/main.o

./objects/cola.o:
	gcc -c ./sources/cola.c -o ./objects/cola.o

./objects/lista.o:
	gcc -c ./sources/lista.c -o ./objects/lista.o

clean:
	-rm ./outputs/*.*
	-rm ./objects/*.o
	-rm ./run