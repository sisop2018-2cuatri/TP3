./run: ./objects/main.o
	gcc ./objects/main.o -o ./run -lpthread

./objects/main.o: ./sources/main.c
	gcc -c ./sources/main.c -o ./objects/main.o

clean:
	-rm ./objects/*.o