main:
	gcc -g -Wall -c main.c -o string.o
run_test:
	gcc -g -Wall test.c string.o -o test && valgrind ./test
