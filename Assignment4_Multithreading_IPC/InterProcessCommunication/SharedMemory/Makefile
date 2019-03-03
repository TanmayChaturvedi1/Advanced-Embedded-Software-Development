#CC = arm-linux-gcc
CC =gcc

all: Producer Consumer

Producer: Producer.c
	$(CC) Producer.c -o Producer -lpthread -lrt -g
	
Consumer: Consumer.c
	$(CC) Consumer.c -o Consumer -lpthread -lrt -g
	
clean:
	rm Producer Consumer
	rm sharedmemlog.txt
