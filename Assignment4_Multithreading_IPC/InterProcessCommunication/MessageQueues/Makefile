#CC = arm-linux-gcc
CC =gcc

all: Client Server

Client: Client.c
	$(CC) Client.c -o Client -lpthread -lrt
	
Server: Server.c
	$(CC) Server.c -o Server -lpthread -lrt
	
clean:
	rm -f Client Server
