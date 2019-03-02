/*
 * Author: Tanmay Chaturvedi
 * Date Created: Feb 27, 2019
 * Course: Advanced Embedded Software Development
 * 
 * This program demonsrates IPC using Sockets.
 * This is the Client Side.
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
#include <arpa/inet.h>

#define	LOG_FILE_NAME	"socketlog.txt"

typedef struct 
{
	char string[20];
	int len;
	int led;
	
}IPCMessage_t;

FILE *fptr;
char *Payload_String[10] = {"Payload:A","Payload:B","Payload:C","Payload:D","LEDStat:O","LEDStat:1","Payload:G","Payload:H","Payload:I","Payload:T"};

void set_sig_handler(void);
long getMicrotime();


int sig_flag = 0;
int socket_fd;

int main()
{
	set_sig_handler();
	struct sockaddr_in server_addr;
	IPCMessage_t client_inbuf, client_outbuf;
	char msg_buf[10];
	char msg_buf1[10];
	char RxBuf[10][10];
	int tan;
	int send_data_len;

printf("Before scket();\n");
	
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_fd < 0)
	{
		perror("socket()");
		exit(1);
	}

	printf("created scket();\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(7000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.10");
	//server_addr.sin_addr.s_addr = inet_addr("10.0.0.10");

	printf("About to Connect() scket();\n");
	/*If connection establishment failed, close socket*/
	if((connect(socket_fd,(struct sockaddr *)&server_addr, sizeof(server_addr))) < 0)
	{
		perror("Socket connect()");
		close(socket_fd);
		exit(1);
	}

printf("after connectscket();\n");

	for( int i =0; i<10; i++)
	{

		strcpy(msg_buf, Payload_String[i]);
		send_data_len = send(socket_fd, msg_buf, 9, 0);
		printf("Data being sent = %s\n",msg_buf);
		//if (send_data_len < 0) {
		//	perror("Client: send");
		//}

		printf("send bytes %d\n", send_data_len);
		sleep(1);
	}

	memset(msg_buf1,0,sizeof(msg_buf1));
	for( int i =0; i < 10; i++)
	{
			if((tan = recv(socket_fd, msg_buf1, 9,0) < 0) && (sig_flag == 0))
			{
				perror("read error");
			}


			strcpy(RxBuf[i],msg_buf1);
			printf("Str = %s\n", msg_buf1);
			printf("Str = %s\n",RxBuf[i]);
			fptr=fopen(LOG_FILE_NAME,"a");
			fprintf(fptr, "Time - %ld\n", getMicrotime());
			printf("Server: RX data\n\n");
			fprintf(fptr, "Server: Receiving data\n");
			fprintf(fptr, "Received data = %s\n", RxBuf[i]);
			fprintf(fptr, "Length of data = %d\n\n", strlen(RxBuf[i]));
			fclose(fptr);
	
	}

//#if 0
	// for( int i =0; i<10; i++)
	// {
	// 	int ret_stat ;
		
	// 	//	memset(&server_addr, 0, sizeof(server_addr));
	// 	memset(msg_buf1, 0 , sizeof(msg_buf1));
	// 	//memset(server_inbuf.string,0,sizeof(server_inbuf.string));
	// 	if(((ret_stat = recv(socket_fd,msg_buf1, 9, 0)) <0 ) && (sig_flag == 0))
	// 	{
	// 		perror("Client: serv()");
	// 	}
	// 	else if(ret_stat == 0){
	// 		printf("Return Status = %d     MsgBuffer = %s\n",ret_stat,msg_buf1);
	// 	}
	// 	else
	// 	{
	// 		printf("Return Status = %d     MsgBuffer = %s\n",ret_stat,msg_buf1);	
	// 		fptr = fopen(LOG_FILE_NAME, "a");
	// 		fprintf(fptr, "Time - %ld\n", getMicrotime());
	// 		fprintf(fptr, "Client: Receiving data\n");
	// 		fprintf(fptr, "Received data = %s\n", msg_buf1);
	// 		fprintf(fptr, "Length of data = %d\n\n", strlen(msg_buf1));
	// 		fclose(fptr);
	// 		//sleep(1);

	// 	}
	// 	//memset(client_inbuf.string, 0, sizeof(client_inbuf.string));
	// }
//#endif

//	}


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
