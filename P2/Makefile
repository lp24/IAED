CFLAGS= -g -Wall -ansi -pedantic -std=gnu99
CC=gcc

proj2: main.o linkedlist.o
	$(CC) $(CFLAGS) -o proj2 main.o linkedlist.o 

main.o: main.c linkedlist.h
	$(CC) $(CFLAGS) -c main.c

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c
	
clean:
	rm -f *.o P2

zip:
	zip P2new.zip main.c linkedlist.c linkedlist.h

run: 
	./proj2
