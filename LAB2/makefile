all: fbroker.o fworker.o broker worker lab2

fbroker.o:
	gcc -c fbroker.c
fworker.o:
	gcc -c fworker.c
broker:
	gcc -o broker fbroker.o broker.c
worker:
	gcc -o worker fworker.o worker.c
lab2:
	gcc -o lab2 lab2.c

run: all
	./lab2 -i prueba_100.txt -o output.txt -n 3 -c 20 -b

clean:
	rm -f *.o broker worker lab2