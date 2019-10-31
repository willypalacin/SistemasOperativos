all: Trinity

main.o: main.c inout.h
	gcc main.c -c -Wall -Wextra

inout.o: inout.h
		gcc inout.c -c -Wall -Wextra

Trinity: main.o inout.o
	gcc main.o inout.o -o Trinity -Wall -Wextra

clean:
	rm *.o Trinity

tar:
	tar -cvzf Trinity.tar.gz *.c *.h makefile
