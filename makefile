run: main
	./a.out -i input.txt -o elpepe -b
main:
	gcc main.c
clean:
	rm -f *.out