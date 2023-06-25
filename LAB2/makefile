all:
	gcc -c fbroker.c
	gcc -c fworker.c
	gcc -o broker fbroker.o broker.c
	gcc -o worker fworker.o worker.c
	gcc -o lab2 lab2.c

run: all
	./lab2 -i prueba_10.txt -o output.txt -w 2 -c 2 -b

clean:
	rm -f main worker