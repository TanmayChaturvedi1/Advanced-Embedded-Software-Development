/*
 * Author: Tanmay Chaturvedi
 * Date Created: Feb 25, 2019
 * Course: Advanced Embedded Software Development
 * 
 * This program creates 3 threads: 1 parent thread and 2 child thread
 * Thread 1 takes a file input "gdb.txt" and prints the occurence of 
 * each character in the command line input file such that their 
 * occurence is less than 100.
 * 
 * Thread 2 runs infinitely with a delay of 100msec and prints proc/stat
 * info in command line input file 
 * 
 * At any time, USR1 and USR2 signals are handled.
 * 
 * [ref1]Time stamp reference: https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/
 * [ref2]Based on code from https://riptutorial.com/posix/example/16306/posix-timer-with-sigev-thread-notification
 * and code provided by Prof Rick Heidebrecht.
 * [ref3]
 *
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void *child_function1( void*);
void *child_function2( void*);

typedef struct
{
	FILE *fptr;
	char* filename;
	
}filestruct_t;

int sig_flag = 0;

const char *message1 = "Child Thread 1";
const char *message2 = "Child Thread 2";


pthread_mutex_t mutex_lock;


static timer_t timerid;
//struct itimerspec trigger;
void thread_handler(union sigval sv);
void handler(int signo, siginfo_t *info, void *extra);
void sig_handler(void);


filestruct_t child, fileinstance;



int main( int argc, char *argv[] )
{
	printf("in Main\n");

	/**add TID, POSIX start and end of main also*/
	if ( argv[1] != NULL )	/*or argc <2*/
	{
		fileinstance.filename = argv[1];
		clock_t start_time, end_time;
    	double cpu_time_used;
    	start_time = clock();
    	cpu_time_used  = (double)start_time/CLOCKS_PER_SEC;
    
	pthread_t thread1, thread2;

	if (pthread_mutex_init(&mutex_lock, NULL) != 0) 
    { 
        printf("\n Mutex init has failed\n"); 
        return 1; 
    } 
    
    sig_handler();

	int ret1, ret2;
	
	ret1 = pthread_create( &thread1, NULL, child_function1, (void*)&fileinstance);
	if( ret1 )
	{
		fprintf( stderr, "Child Thread 1 not created, Error Code: %d\n", ret1);
	}

	ret2 = pthread_create( &thread2, NULL, child_function2, (void*)&fileinstance );
	if( ret2 )
	{
		fprintf( stderr, "Child Thread 2 not created, Error Code: %d\n", ret2);
	}
	
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	
	pthread_mutex_destroy(&mutex_lock); 
	
	printf(" Main Thread Exited Successfully \n");
	
	return 0;
	}
	else{
		 printf(" ERROR: Enter the file name along with ./final \n");
		return -1;
	}
}

void mask_sig(void)
{
	sigset_t mask;
	sigemptyset(&mask); 
        sigaddset(&mask, SIGUSR1); 
                
        pthread_sigmask(SIG_BLOCK, &mask, NULL);
        
}



void *child_function1( void* argv )
{
	
	mask_sig();
	//pthread_mutex_lock(&mutex_lock);
	clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();
    cpu_time_used  = (double)start_time/CLOCKS_PER_SEC;

	FILE *fptr_gdb;
	int file_char;
	int char_array[26] = {0};
	pid_t linux_tid = syscall(SYS_gettid);
	pthread_t posix_tid = pthread_self();
	filestruct_t *child1_file = (filestruct_t *)argv;

	fptr_gdb = fopen( "gdb.txt", "r" );
	if (fptr_gdb != NULL)
	{
	do 
	{
		file_char = fgetc (fptr_gdb);
		if ((file_char >= 65) && (file_char <=90))  
		{
			char_array[file_char - 65]++;
		}
		else if ((file_char >= 97) && (file_char <= 122))
		{
			char_array[file_char - 97]++;
		}
		
	}
		while (!feof(fptr_gdb));
	}
	fclose(fptr_gdb);

	pthread_mutex_lock(&mutex_lock);
	if ((child.fptr = fopen(child1_file->filename, "a" )) != NULL)
	{	
		fprintf(child.fptr, "\nStart Time = %f\n", (double)start_time/CLOCKS_PER_SEC);
		fprintf(child.fptr, "****\nChild1 thread work started\n\n");
		fputs( message1, child.fptr );
		fprintf(child.fptr, "\nLinux Thread id = %d \n", linux_tid);
		fprintf(child.fptr, "Posix Thread id = %ld \n", posix_tid);
		
		for (int i =0; i<26; i++)
		{
			if (char_array[i] < 100)
				fprintf(child.fptr,"Character = %c and occurence times = %d\n", i+97, char_array[i]);
		}
			fprintf(child.fptr, "****\nChild1 thread work completed\n\n");
			fprintf(child.fptr,"End Time = %f\n", ((double)clock()/CLOCKS_PER_SEC));
			fclose( child.fptr );
			pthread_mutex_unlock(&mutex_lock); 
	}
	else printf(" Child Thread 1 created, File not opened \n");

	printf("exit child 1\n");
}



void thread_handler(union sigval sv)
{
	//printf("in treadh handler");
	char *s=sv.sival_ptr;
	struct timespec thTimespec;
	clock_t start_time;
	start_time = clock();

	printf("100msec elapsed\n");
	//char* filename;
	pid_t linux_tid = syscall(SYS_gettid);
	pthread_t posix_tid = pthread_self();

	pthread_mutex_lock(&mutex_lock);
	FILE *file_proc_stat;
	file_proc_stat=popen("cat /proc/stat | grep 'cpu'","r");


	child.fptr=fopen("tanmay.txt","a");

	if(child.fptr !=NULL)
	{
		fprintf(child.fptr,"\nStart Time = %f\n", ((double)start_time/CLOCKS_PER_SEC));
		fprintf(child.fptr, "\nLinux Thread id = %d \n", linux_tid);
		fprintf(child.fptr, "Posix Thread id = %ld \n", posix_tid);
			

		while(!feof(file_proc_stat))
		{	
			char cpu_util;
			cpu_util = fgetc(file_proc_stat);

			fprintf(child.fptr,"%c",cpu_util);

		}
		fprintf(child.fptr,"End Time = %f\n", ((double)clock()/CLOCKS_PER_SEC));
		fclose( child.fptr );
		pclose(file_proc_stat);
		pthread_mutex_unlock(&mutex_lock);
	}

	
}



void *child_function2( void* argv )
{
	//printf("in Child2\n");
	struct sigevent sev;
	struct timespec mainTimeSpec;
	struct itimerspec trigger;	
	char info[] = "100 milisecond elapsed.";
	memset(&sev, 0, sizeof(struct sigevent));
    memset(&trigger, 0, sizeof(struct itimerspec));
    /* 
         * Set the notification method as SIGEV_THREAD:
         *
         * Upon timer expiration, `sigev_notify_function` (thread_handler()),
         * will be invoked as if it were the start function of a new thread.
         *
         */
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = &thread_handler;
	sev.sigev_value.sival_ptr = &info;
	
	timer_create(CLOCK_REALTIME, &sev, &timerid);
	trigger.it_value.tv_nsec = 100000000; //10^8 nano sec is 100msec
	trigger.it_interval.tv_nsec = 100000000;
	timer_settime(timerid, 0, &trigger, NULL);
	while(! sig_flag );
	timer_delete(timerid);

}


void sig_handler(void)
{
    struct sigaction action;
    action.sa_flags = SA_SIGINFO; 
    action.sa_sigaction = handler;
 
    if (sigaction(SIGUSR1, &action, NULL) == -1)
    { 
        perror("sigusr: sigaction");
        _exit(1);
    }

}


void handler(int signo, siginfo_t *info, void *extra) 
{
	/*global flag which is checked in child2 thread
	  and set whenever USR singal is handled
	*/
	sig_flag = 1;
}