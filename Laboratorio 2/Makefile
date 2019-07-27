CC=gcc


CFLAGS=-c -Wall

all: laboratorio2

laboratorio2: main.o funciones.o
	$(CC) main.o funciones.o -o laboratorio2 -pthread -lm

main.o: main.c
	$(CC) $(CFLAGS) main.c

funciones.o: funciones.c
	$(CC) $(CFLAGS) funciones.c

clean:
	rm -rf *o laboratorio2
