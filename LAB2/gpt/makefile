all: lab2 worker broker
run: lab2 broker
	./lab2 -i prueba_10.txt -o output.txt -w 10 -c 20 -b
lab2:
	gcc -g -o lab2 lab2.c
worker: fworker.o
	gcc -g -o worker fworker.o worker.c
broker: fbroker.o
	gcc -g -o broker fbroker.o broker.c
fbroker.o: fbroker.c
	gcc -g -c fbroker.c
fworker.o: fworker.c
	gcc -g -c fworker.c
clean:
	rm -f *.o lab2 worker broker