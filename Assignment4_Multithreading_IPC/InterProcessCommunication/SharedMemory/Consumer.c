/*
 * Author: Tanmay Chaturvedi
 * Date Created: Feb 27, 2019
 * Course: Advanced Embedded Software Development
 * 
 * This program demonsrates IPC using Shared Memory Resource.
 * This is the Consumer Side.
 * Reference[1]: https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
 * Reference[2]: For timer module : https://gist.github.com/sevko/d23646ba07c77c15fde9
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
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/mman.h>



#define	SIZE	(2048)
#define	SEMAPHORE	"sem_proc"
#define	SEMAPHORE_1	"sem_cons"
#define	SHARED_MEM 	"shared_mem_resource"
#define	LOG_FILE_NAME "sharedmemlog.txt"


typedef struct 
{
	char string[20];
	int len;
	int led;
	
}IPCMessage_t;

FILE *fptr;
char *Payload_String[10] = {"Payload:A","Payload:B","Payload:C","Payload:D","LEDStat:0","LEDStat:1","Payload:G","Payload:H","Payload:I","Payload:T"};
void set_signal_handler(void);
long getMicrotime();


	
int main(){

	IPCMessage_t *tx_data;

	set_signal_handler();

	int sm_fd = shm_open(SHARED_MEM, O_CREAT | O_RDWR, 0666);
	if(sm_fd < 0)
	{
		perror("Client: shm_open()");
		exit(1);
	}

	int sm_size = sizeof(IPCMessage_t);

	int trunc_val = ftruncate(sm_fd, sm_size);
	if (trunc_val < 0)
	{
		perror("Client: ftruncate()");
		exit(1);
	}

	/*Opening Semaphores*/

	sem_t * sem1 = sem_open(SEMAPHORE, O_CREAT, 0666, 0);
	if( sem1 == NULL)
	{
		perror("Client: sem_open()");
	}

	sem_t * sem2 = sem_open(SEMAPHORE_1, O_CREAT, 0666, 0);
	if( sem2 == NULL)
	{
		perror("Client: sem_open()");
	}



	/*memory mapping*/

	tx_data = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sm_fd, 0);	/*for sufficient memory availability, \
																									I used SIZE instead of sm_size value */

	if(tx_data < 0)
	{
		perror("Producer: mmap()");
	}

	//IPCMessage_t data* = &shared_mem;

	/*Data Communication begins*/
	for( int i = 0; i< 10; i++ )
	{
		sem_wait(sem1);
		fptr = fopen(LOG_FILE_NAME,"a");
		fprintf(fptr,"\n\n in Consumer (Old data) \n\n");
		fprintf(fptr, "Retrieved data = %s\n", (char *)tx_data->string);
		fprintf(fptr, "Retrieved data len = %ld\n", strlen(tx_data->string));
		fprintf(fptr, "Retrieved LED status = %d\n", tx_data->led);
		fclose(fptr);

		sprintf(tx_data, "%s\n", (char *)Payload_String[i]);
		sprintf(tx_data, "%ld\n", strlen(Payload_String[i]));
		sprintf(tx_data, "%d\n", i%2);

		sem_post(sem2);
//
		 strcpy(tx_data->string, Payload_String[i]);
		 tx_data->len = strlen(tx_data->string);
		 tx_data->led = i%2;

		// sprintf(shared_mem, &tx_data, sizeof(tx_data));

	}



		shm_unlink(SEMAPHORE);
		sem_destroy(sem1);
		shm_unlink(SEMAPHORE_1);
		sem_destroy(sem2);
		shm_unlink(SHARED_MEM);
	return 0;
}

void signal_handler(int signo, siginfo_t *info, void *extra) 
{
    fptr = fopen(LOG_FILE_NAME, "a");
    fprintf(fptr, "Time - %ld\n", getMicrotime());
    fprintf(fptr, "SIG Detected, Exiting!\n");
    fclose(fptr);


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




