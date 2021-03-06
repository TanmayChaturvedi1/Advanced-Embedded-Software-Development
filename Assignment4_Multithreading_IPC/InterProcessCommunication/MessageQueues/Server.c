/*
 * Author: Tanmay Chaturvedi
 * Date Created: Feb 27, 2019
 * Course: Advanced Embedded Software Development
 * 
 * This program demonsrates IPC using Posix Message Queues.
 * This is the Server Side.
 * Refernce[1]: https://www.softprayog.in/programming/interprocess-communication-using-posix-message-queues-in-linux
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define CLIENT_QUEUE_NAME   "/client_process11"
#define SERVER_QUEUE_NAME   "/server_process11"
#define	LOG_FILE_NAME		"posixqueuelog.txt"
#define QUEUE_PERMISSIONS	(0664)
#define MAX_MESSAGES		(10)	/*Max message in queue*/
#define MAX_MSG_SIZE		(256)	/*Individual Message Size*/
#define MSG_BUFFER_SIZE 	MAX_MSG_SIZE + 10

typedef struct 
{
	char string[20];
	int len;
	int led;
}IPCmessage_t;

void set_signal_handler(void);
long getMicrotime();

FILE *fptr;
mqd_t qd_server, qd_client;	/*Queue Descriptor for Client/Server*/

    int main()
{
	set_signal_handler();
	struct mq_attr posixq_attr;
    IPCmessage_t in_buf, out_buf; 
	//IPCmessage_t in_buf[MAX_MSG_SIZE], out_buf[MAX_MSG_SIZE];
	char *Payload_String[10] = {"Payload:A","Payload:B","Payload:C","Payload:D","Payload:E","Payload:F","Payload:G","Payload:H","Payload:I","Payload:T"};

   printf("Size %ld", sizeof(in_buf));
    posixq_attr.mq_flags = 0;
    posixq_attr.mq_maxmsg = MAX_MESSAGES;
    posixq_attr.mq_msgsize = sizeof(in_buf);
    posixq_attr.mq_curmsgs = 0;


/*
    fptr = fopen(LOG_FILE_NAME, "a");
    if( fptr == NULL )
    {
    	printf(" Error: Unable to open file\n");
    }*/

    //fprintf(fptr, "/*Add time stamp info and posix id*/");

    qd_client = mq_open( CLIENT_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &posixq_attr );
    qd_server = mq_open( SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &posixq_attr );
    if( qd_server == -1 )
    {
            perror ("Server: mq_open()");
            exit (EXIT_FAILURE);
    }
//    	int resp = mq_send( qd_server, (const char *)&out_buf, sizeof(out_buf[i]), 0 );

    for (int  i = 0; i< MAX_MESSAGES; i++)
    {
    	/*char * myptr;
    	IPCmessage_t new;
    	myptr = (char *)&new;*/
      //  memset(in_buf, 0 , sizeof(in_buf));
    	int received = mq_receive( qd_server, (char*)&in_buf, sizeof(in_buf), 0 );
    	fptr = fopen(LOG_FILE_NAME, "a");
        fprintf(fptr,"Time - %ld\n", getMicrotime());
    	fprintf(fptr,"In Server: Data= %s\n",in_buf.string);
        fprintf(fptr,"In Server: Data Length= %d\n",in_buf.len);
        fprintf(fptr,"In Server: Led Status= %d\n",in_buf.led);
        fclose(fptr);
    //	printf("In Server: Len=, %d\n",new.len);
    	//printf("In Server: LED=, %d\n",(new.led));
    	sleep(1);
    	if ( received == -1 )
    	{
    		perror("Server: mq_receive()");
    		exit (EXIT_FAILURE);
    	}

    }

    for (int  i = 0; i< MAX_MESSAGES; i++)
    {
    //	strcpy(out_buf.string, Payload_String[i]);
        strcpy(out_buf.string , Payload_String[i]);
    	out_buf.len = strlen(out_buf.string);
    	out_buf.led = i%2;

    	int resp = mq_send( qd_client, (const char *)&out_buf,sizeof(out_buf), 0);
    	if ( resp == -1 )
    	{
    		perror("Server: mq_send() Not ablet to send data to client");
    		exit (EXIT_FAILURE);
    	}


    }

    mq_close( qd_client );
    mq_close( qd_server );

    if (fclose( fptr ) != 0)
    {
    	perror("flose()");
    	exit (EXIT_FAILURE);
    }

    return 0;
}


void signal_handler(int signo, siginfo_t *info, void *extra) 
{
    fptr = fopen(LOG_FILE_NAME, "a");
    fprintf(fptr, "Time - %ld", getMicrotime());
    fprintf(fptr, "SIG Detected, Exiting!\n");
    fclose(fptr);
    
    mq_close( qd_client );
    mq_close( qd_server );

    int resp = fclose(fptr);
    if(resp != 0)
        perror("fclose() in SIGEVENT");

    exit(0);
}

void set_signal_handler(void)
{
        struct sigaction action;
 
        action.sa_flags = SA_SIGINFO; 
        action.sa_sigaction = signal_handler;
 
        if (sigaction(SIGINT, &action, NULL) == -1) { 
            perror("sigusr1: sigaction");
            _exit(1);
        }
}

long getMicrotime(){
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}