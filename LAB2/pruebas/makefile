all:
	gcc -o lab2 lab2.c
	gcc -o broker broker.c
	gcc -c funciones.c
	gcc -o worker funciones.o worker.c

run: all
	./lab2 -i prueba_10.txt -o output.txt -w 1 -c 10 -b

clean:
	rm -f main worker