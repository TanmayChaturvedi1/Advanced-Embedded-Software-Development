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


int main()
{
	int pipe1fds[2], pipe2fds[2];	/*pipe1fds[0] is for reading, pipe1fds[1] is for writing */
	int ret1, ret2;
	int pid;
	FILE *fptr;

	typedef struct 
	{
		char*	string;
		int 	len;	
	}IPCmessage_t;

	char *Payload_String[10] = {"Payload:A","Payload:B","Payload:C","Payload:D","Payload:E","Payload:F","Payload:G","Payload:H","Payload:I","Payload:J"};
	char *Receive_String[10] = {0};
    IPCmessage_t str_obj, str_obj_1 ;

    str_obj.len = 9;

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

		//close( pipe1fds[0] );	/*Read Closed for Pipe 1*/
		//close( pipe2fds[1] );	/*Write Closed for Pipe 2*/ 
		for( int i=0; i<10; i++)
		{
			str_obj.string = Payload_String[i];
	int		a = 	write( pipe1fds[1], str_obj.string, str_obj.len );
	//printf("%d\n", a);
	sleep(1);
		}

		for( int i=0; i<10; i++)
		{
		read( pipe2fds[0], str_obj_1.string, str_obj.len ); 
		
		printf("In Parent: Message is %s\n", str_obj_1.string );
		}

	}

	else
	{
		//close( pipe1fds[1] );	/*Write Closed for Pipe 1*/
		//close( pipe2fds[0] );	/*Read Closed for Pipe 2*/ 

	//sleep(2);
	for( int  i=0; i<10; i++)
	{
		
		int b = read( pipe1fds[0], &str_obj.string, str_obj.len );
		printf("child read %d\n", b);
		printf("In Child: Message is %s\n", &str_obj.string );
	}
	for( int i=0; i<10; i++)
	{
		str_obj_1.string = Payload_String[i];
		int c = write( pipe2fds[1], &str_obj_1.string, str_obj.len );
		printf("%d\n", c);
	}	 
		


	}

	return 0;






}



