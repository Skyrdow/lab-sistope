all: lab2 worker broker
lab2:
	gcc -g -o lab2 lab2.c
worker: fworker.o
	gcc -g -o worker fworker.o worker.c
broker: fbroker.o
	gcc -g -o broker fbroker.o broker.c
fbroker.o: fbroker.c
	gcc -c fbroker.c
fworker.o: fworker.c
	gcc -c fworker.c
clean:
	rm -f *.o lab2 worker broker