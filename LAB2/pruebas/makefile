all:
	gcc -o main main.c
	gcc -c funciones.c
	gcc -o worker funciones.o worker.c
clean:
	rm -f main worker