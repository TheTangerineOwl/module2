all: 2-1-exec

2-1-exec: main.o contacts.o
	gcc main.o contacts.o -o 2-1-exec

main.o: main.c
	gcc -c main.c

contacts.o: contacts.c
	gcc -c contacts.c

clean:
	rm -rf *.o 2-1-exec