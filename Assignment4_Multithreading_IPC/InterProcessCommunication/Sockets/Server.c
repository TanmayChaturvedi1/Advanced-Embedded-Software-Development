/*
 * Author: Tanmay Chaturvedi
 * Date Created: Feb 27, 2019
 * Course: Advanced Embedded Software Development
 * 
 * This program demonsrates IPC using Sockets.
 * This is the Server Side.
 * Reference[1]: http://www.it.uom.gr/teaching/distrubutedSite/dsIdaLiu/labs/lab2_1/sockets.html
 * Reference[2]: https://www.youtube.com/watch?v=pFLQmnmDOo
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h>


#define	SERVER_HOST	"Localhost"
#define	LOG_FILE_NAME	"socketlog.txt"
#define	PORT_NUM	(7000)
//#define	INET_ADDR_VAL	("192.168.128.10")


typedef struct 
{
	char string[20];
	int len;
	int led;
	
}IPCMessage_t;

FILE *fptr;
char *Payload_String[10] = {"Payload:A","Payload:B","Payload:C","Payload:D","LEDStat:1","LEDStat:O","Payload:G","Payload:H","Payload:I","Payload:T"};
char *RxBuf[10] = {0};

void set_sig_handler(void);
long getMicrotime();

int socket_fd;
int check_stat;
//int send_data_len;
int sig_flag = 0;

// IPCMessage_t server_outbuf, server_inbuf;

int sock_stat;

int main()
{
	set_sig_handler();
	IPCMessage_t server_outbuf, server_inbuf;
	int tan;
	int send_data_len;
	struct sockaddr_in server_addr;
//	set_sig_handler();
	
	
//	int send_data_len;

	socket_fd = socket(AF_INET, SOCK_STREAM,0);

	if(socket_fd < 0)
	{
		perror("Server: socket()");
		exit(1);
	}


	//memset(msg_buf,0,sizeof(msg_buf));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(7000);


//	socket_fd = socket(AF_INET, SOCK_STREAM,0);

	// if(socket_fd < 0)
	// {
	// 	perror("Server: socket()");
	// 	exit(1);
	// }

	if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		perror("Server: bind()");
		exit(1);
	}

	listen(socket_fd, 5);

	sock_stat = accept(socket_fd, (struct sockaddr *) 0,0);

	if( sock_stat == -1)
	{
			perror("Serve: accept()");

	}
	//char str[10];
	char msg_buf[9];
	char tx_buf[10];
	for( int i =0; i < 10; i++)
	{	
			if((tan = recv(sock_stat, msg_buf, 9,0) < 0) && (sig_flag == 0))
			{
				perror("read error");
				//return 1;
			}


			RxBuf[i] = msg_buf;
			printf("Str = %s\n", msg_buf);
			printf("Str = %s\n",RxBuf[i]);
			fptr=fopen(LOG_FILE_NAME,"a");
			fprintf(fptr, "Time - %ld\n", getMicrotime());
			printf("Server: RX data\n\n");
			fprintf(fptr, "Server: Receiving data\n");
			fprintf(fptr, "Received data = %s\n", RxBuf[i]);
			fprintf(fptr, "Length of data = %d\n\n", strlen(RxBuf[i]));
			fclose(fptr);
	
	}



	
	for( int i =0; i<10; i++)
	{
		memset(tx_buf,0,sizeof(tx_buf));
		strcpy(tx_buf, Payload_String[i]);
		printf("Message buffer (S->C) = %s\n",tx_buf);

		send_data_len = send(sock_stat, tx_buf, 9, 0);
		printf("Data being sent = %s Data length = %d \n",tx_buf,send_data_len);

		if (send_data_len < 0) {
			perror("Client: send");
		}

		printf("send bytes %d\n", send_data_len);
		sleep(1);
	}
	
	// for( int i =0; i < 10; i++)
	// {
	// 			//strcpy(msg_buf, Payload_String[i]);
	// 			strcpy(tx_buf, Payload_String[i]);
	// 			send_data_len = send(socket_fd,tx_buf, 9 , 0);
	// 			if (send_data_len < 0){
	// 				perror("Server: send()");
	// 			}
	// 			printf("String being sent from server to client= %s\n",tx_buf);
	// 			printf("send bytes %d\n", send_data_len);
	// 			sleep(1);

	// 			//sleep(1);
	// }

	return 0;


}





void sig_handler(int signo, siginfo_t *info, void *extra)
{
	sig_flag = 1;
	fptr = fopen(LOG_FILE_NAME, "a");
	fprintf(fptr, "Time - %ld", getMicrotime());
	fprintf(fptr, "SIG Detected, Exiting!\n");
	fclose(fptr);
	close(socket_fd);
	exit(0);
}

void set_sig_handler(void)
{
    struct sigaction action;
    action.sa_flags = SA_SIGINFO; 

    action.sa_sigaction = sig_handler;
 
    if (sigaction(SIGINT, &action, NULL) == -1)
    { 
        perror("sigusr: sigaction");
        _exit(1);
    }
}

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}




		// else 
		// 	// int check = recv(sock_stat, msg_buf, 10, 0);
		// 	// if( check < 0  && sig_flag == 0 )
		// 	// {
		// 	// 	perror("Server: recv()");
		// 	// }
		// }

		// else
		// {
		// 	memset(msg_buf,0,sizeof(msg_buf));
		// 	fptr = fopen(LOG_FILE_NAME, "a");
		// 	fprintf(fptr, "Time - %ld\n", getMicrotime());
		// 	printf("Server: RX data\n\n");
		// 	fprintf(fptr, "Server: Receiving data\n");
		// 	fprintf(fptr, "Received data = %s\n", msg_buf);
		// 	fprintf(fptr, "Length of data = %ld\n", strlen(msg_buf));
		// 	fclose(fptr);
		// 	sleep(1);

		// }


		// server.string = Payload_String[i];
		// send_data_len = send(socket_fd, server.string, strlen(server.string), 0);
		// fptr = fopen(LOG_FILE_NAME, "a");
		// fprintf(fptr, "Time - %ld\n", getMicrotime());
		// fprintf(fptr, "Server: Sending data\n\n");
		// printf("Server: Sending data\n\n");
		// fclose(fptr);
