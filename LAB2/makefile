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

clean:
	rm -f *.o broker worker lab2