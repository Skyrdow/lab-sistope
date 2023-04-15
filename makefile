test10: main
	./main -i prueba_10.txt -o salida_10.txt
test100: main
	./main -i prueba_100.txt -o salida_100.txt
test500: main
	./main -i prueba_500.txt -o salida_500.txt
test1000: main
	./main -i prueba_1000.txt -o salida_1000.txt
test10_b: main
	./main -i prueba_10.txt -o salida_10.txt -b
test100_b: main
	./main -i prueba_100.txt -o salida_100.txt -b
test500_b: main
	./main -i prueba_500.txt -o salida_500.txt -b
test1000_b: main
	./main -i prueba_1000.txt -o salida_1000.txt -b
main: main.c funciones.c
	gcc -o main main.c funciones.c -Wall
clean:
	rm -f main