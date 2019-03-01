#Makefile
CC = gcc

final: IPC_pipes.o
	$(CC) -o final IPC_pipes.o

IPC_pipes.o: IPC_pipes.c
	$(CC) -c IPC_pipes.c

clean:	
	rm final IPC_pipes.o

