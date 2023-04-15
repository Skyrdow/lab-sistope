test10: lab1
	./lab1 -i prueba_10.txt -o salida_10.txt
test100: lab1
	./lab1 -i prueba_100.txt -o salida_100.txt
test500: lab1
	./lab1 -i prueba_500.txt -o salida_500.txt
test1000: lab1
	./lab1 -i prueba_1000.txt -o salida_1000.txt
test10_b: lab1
	./lab1 -i prueba_10.txt -o salida_10.txt -b
test100_b: lab1
	./lab1 -i prueba_100.txt -o salida_100.txt -b
test500_b: lab1
	./lab1 -i prueba_500.txt -o salida_500.txt -b
test1000_b: lab1
	./lab1 -i prueba_1000.txt -o salida_1000.txt -b
lab1: lab1.c funciones.c
	gcc -o lab1 lab1.c funciones.c -Wall
clean:
	rm -f lab1