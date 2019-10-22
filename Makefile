all: PRACTICASO

main.o: main.c inout.h
	gcc main.c -c -Wall -Wextra

inout.o: inout.h
		gcc inout.c -c -Wall -Wextra

PRACTICASO: main.o inout.o
	gcc main.o inout.o -o PRACTICASO.exe -Wall -Wextra

clean:
	rm *.o PRACTICASO.exe

tar:
	tar -cvzf PRACTICASO.tar.gz *.c *.h makefile
