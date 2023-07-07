all: funciones.o lab3

funciones.o:
	gcc -c funciones.c
lab3: funciones.o
	gcc -o lab3 lab3.c funciones.o

clean:
	rm -f *.o lab3