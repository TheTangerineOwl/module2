all: 2-1-exec

2-1-exec: contactsIO.o main.o contacts.o
	gcc contactsIO.o main.o contacts.o -o 2-1-exec

main.o: main.c
	gcc -c main.c

contacts.o: contacts.c
	gcc -c contacts.c

contactsIO.o: contactsIO.c
	gcc -c contactsIO.c

.PHONY: clean
clean:
	rm -rf *.o 2-1-exec