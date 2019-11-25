all: Trinity

main.o: main.c inout.h
	gcc main.c -c -Wall -Wextra

inout.o: inout.h
		gcc inout.c -c -Wall -Wextra

conexion.o: conexion.h
	gcc conexion.c -c -lpthread -Wall -Wextra

Trinity: main.o inout.o conexion.o
	gcc main.o inout.o conexion.o -lpthread -o  Trinity -Wall -Wextra

clean:
	rm *.o Trinity

tar:
	tar -cvzf Trinity.tar.gz *.c *.h makefile
