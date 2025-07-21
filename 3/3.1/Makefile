all: 3-1-exec

3-1-exec: main.o stats.o
	gcc main.o stats.o -o 3-1-exec

main.o: main.c
	gcc -c main.c

stats.o: stats.c
	gcc -c stats.c

clean:
	rm -rf *.o 3-1-exec