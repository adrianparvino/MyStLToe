core:
	gcc -g -Wall -c core.c -o core.o
lib: core.o file.o
	ar rcs libmystltoe.a core.o
