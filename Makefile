all: PRACTICASO

main.o: main.c ficheros.h
	gcc main.c -c -Wall -Wextra

ficheros.o: ficheros.h
		gcc ficheros.c -c -Wall -Wextra

PRACTICASO: main.o ficheros.o
	gcc main.o ficheros.o -o PRACTICASO.exe -Wall -Wextra

clean:
	rm *.o PRACTICASO.exe

tar:
	tar -cvzf PRACTICASO.tar.gz *.c *.h makefile
