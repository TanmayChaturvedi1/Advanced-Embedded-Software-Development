#Makefile
CC = gcc

final:	multi_thread.o
	$(CC) -o final multi_thread.o -pthread

multi_thread.o:	multi_thread.c
	$(CC) -c multi_thread.c -pthread

clean:	
	rm final multi_thread.o

