all: lab2 broker

run: lab2 broker
	./lab2 -i input.txt -o output.txt -n 10 -c 20 -b
lab2: lab2.c broker
	gcc lab2.c -o lab2

broker: broker.c
	gcc broker.c -o broker

clean:
	rm -f *.o