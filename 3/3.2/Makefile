all: 3-2-exec

3-2-exec: main.o ipnet.o
	gcc main.o ipnet.o -o 3-2-exec

main.o: main.c
	gcc -c main.c

ipnet.o: ipnet.c
	gcc -c ipnet.c

clean:
	rm -rf *.o 3-2-exec