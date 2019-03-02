/*
 * Author: Tanmay Chaturvedi
 * Date Created: Feb 27, 2019
 * Course: Advanced Embedded Software Development
 * 
 * This program creates 2 processes and performs inter-communication between
 * the 2 processes as per the Assignment Requirements
 * Reference[1]: https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_pipes.htm
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define	LOG_FILE_NAME	("pipeslog.txt")

int main()
{
	int pipe1fds[2], pipe2fds[2];	/*pipe1fds[0] is for reading, pipe1fds[1] is for writing */
	int ret1, ret2;
	int pid;
	
	typedef struct 
	{
		char*	string;
		int 	len;	
		int 	led;
	}IPCmessage_t;

long getMicrotime();

	char *Payload_String[10] = {"Payload:A","Payload:B","Payload:C","Payload:D","Payload:E","Payload:F","Payload:G","Payload:H","Payload:I","Payload:T"};
	//char Receive_String[10][10]={0};
	FILE *fptr;

    IPCmessage_t str_obj, str_obj_1 ;

   // str_obj.len = 10;
    //str_obj_1.len = 10;

    char buff[20];
    int str_len, led_state;

	ret1 = pipe( pipe1fds );	/*Creates a unidirectional pipe*/

	if( ret1 == -1 )
	{
		printf("Pipe1 not created\n");
		return 1;
	}

	ret2 = pipe( pipe2fds );

	if( ret2 == -1 )
	{
		printf("Pipe1 not created\n");
		return 1;	
	}

	pid = fork();

	if( pid > 0 )	/*Parent Process*/
	{

		close( pipe1fds[0] );	/*Read Closed for Pipe 1*/
		close( pipe2fds[1] );	/*Write Closed for Pipe 2*/ 
		for( int i=0; i<10; i++)
		{

			str_obj.string = Payload_String[i];
			int	a = 	write( pipe1fds[1], str_obj.string, sizeof(buff) );
			printf("Parent->Str Byte = %s\n", str_obj.string);
			sleep(1);


		}

		for( int i=0; i<10; i++)
		{
			//str_len = 
			//sprintf(str_obj.string, )
			str_obj.len = strlen(Payload_String[i]);
			str_len = str_obj.len;
			
			int	b = 	write( pipe1fds[1], &str_obj.len, sizeof(int) );
		//	printf("Parent->len Byte = %d\n", b);
			sleep(1);
		}


		for( int i=0; i<10; i++)
		{
			//str_obj.len = sizeof(str_obj.len);
			str_obj.led = i%2;	/*to alternate between LED 0 and LED 1*/
			int	c = 	write( pipe1fds[1], &str_obj.led, sizeof(int) );
			printf("Parent->LED Byte = %d\n", c);
			sleep(1);
		}

		for( int  i=0; i<10; i++)
		{

			fptr = fopen(LOG_FILE_NAME, "a");
			memset(buff, 0 , sizeof(buff));
			int d = read( pipe2fds[0], &buff, sizeof(buff) );
			printf("parent read %d\n", d);
			printf("In Parent: Message is %s\n", buff );
			fprintf(fptr, "Time - %ld\n", getMicrotime());
			fprintf(fptr, "In Parent: Received Message is %s\n\n", buff);
			fclose(fptr);
			//sleep(1);
		}
		for( int  i=0; i<10; i++)
		{
			fptr = fopen(LOG_FILE_NAME, "a");
			int e = read( pipe2fds[0], &str_len, sizeof(int) );
			printf("parent read %d\n", e);
			printf("In Parent: Len is %d\n", str_len );
			fprintf(fptr, "Time - %ld\n", getMicrotime());
			fprintf(fptr, "In Parent: Received Message Length is %d\n\n", str_len);
			fclose(fptr);
			sleep(1);
		}

		for( int  i=0; i<10; i++)
		{
			fptr = fopen(LOG_FILE_NAME, "a");	
			int f = read( pipe2fds[0], &led_state, sizeof(int) );
			printf("parent read %d\n", f);
			printf("In Parent: Len is %d\n", led_state );
			fprintf(fptr, "Time - %ld\n", getMicrotime());
			fprintf(fptr, "In Parent: Received LED Command is %d\n\n", led_state);
			fclose(fptr);
			sleep(1);
		}

	}

	else
	{
		close( pipe1fds[1] );	/*Write Closed for Pipe 1*/
		close( pipe2fds[0] );	/*Read Closed for Pipe 2*/ 
		for( int  i=0; i<10; i++)
		{
			fptr = fopen(LOG_FILE_NAME, "a");
			memset(buff, 0, sizeof(buff));
			int g = read( pipe1fds[0], &buff, sizeof(buff));
			//printf("child read %d\n", g);
			printf("In Child: Message is %s\n", buff );
			fprintf(fptr, "Time - %ld\n", getMicrotime());
			fprintf(fptr, "In Child: Received Message is %s\n\n", buff);
			fclose(fptr);

			sleep(1);
		}

		for( int  i=0; i<10; i++)
		{
			fptr = fopen(LOG_FILE_NAME, "a");
			int h = read( pipe1fds[0], &str_len, sizeof(str_len ));
			//printf("child read %d\n", h);
			printf("In Child: String Length is %d\n", str_len );
			fprintf(fptr, "Time - %ld\n", getMicrotime());
			fprintf(fptr, "In Child: Received Message Length is %d\n\n", str_len);
			fclose(fptr);
			sleep(1);
		}

		for( int  i=0; i<10; i++)
		{

			fptr = fopen(LOG_FILE_NAME, "a");
			int i = read( pipe1fds[0], &led_state, sizeof(led_state) );
			printf("child read %d\n", i);
			printf("In Child: LED state is %d\n", led_state );

			fprintf(fptr, "Time - %ld\n", getMicrotime());
			fprintf(fptr, "In Child: Received Led State is %d\n\n", led_state);
			fclose(fptr);
			sleep(1);
		}

		for( int i=0; i<10; i++)
		{
			str_obj_1.string = Payload_String[i];
			int	j = write( pipe2fds[1], str_obj_1.string, sizeof(buff) );
			printf("%d\n", j);
			sleep(1);
		}
		
		for( int i=0; i<10; i++)
		{
			str_obj_1.len = strlen(Payload_String[i]);
			int	k = write( pipe2fds[1], &str_obj_1.len, sizeof(int) );
			printf("%d\n", k);
			sleep(1);
		}
		
		for( int i=0; i<10; i++)
		{

			str_obj_1.led = i%2;	/*to alternate between LED 0 and LED 1*/
			//str_obj_1.string = sizeof(str_obj_1.len);
			int	l = write( pipe2fds[1], &str_obj_1.led, sizeof(int));
			printf("%d\n", l);
			sleep(1);
		}
		
	}

	return 0;

}



long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}


