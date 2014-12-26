core:
	gcc -g -Wall -c core.c -o core.o
lib:
	ar rcs mystltoe.a core.o
