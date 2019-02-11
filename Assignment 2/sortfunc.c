/* Code by Tanmay Chaturvedi
 * 
 * References for Time/Date:https://www.includehelp.com/c-programs/system-date-time-linux.aspx
 * */


#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>



#define	ARR_LEN	(256)

int main(void)
{

	int usort_buf[ ARR_LEN ];	/*Unsorted Buffer*/
	int sort_buf[ ARR_LEN ];	/*Sorted Buffer*/
	
	int i;
	
	/*Validation of input parameters*/
	/*Arr len more than 2 for sorting and no null pointers*/
	if( ( ARR_LEN >= 256 ) && /* ( usort_buf != NULL ) && */ ( sort_buf != NULL ) )
	{
		printf(" IN USER SPACE \n", ARR_LEN );
		printf(" Size of array = %d\n", ARR_LEN );

		printf( " unsorted array: \n");
		for ( i = 0; i < ARR_LEN; i++ )
		{
			usort_buf[i] = rand()%1000; /*Limiting random number from 0 - 999*/
			
			printf(" %d", usort_buf[i] );
		}
		
		printf( " Syscall Entering..\n");
		int return_type = syscall(399, NULL, ARR_LEN, sort_buf );	/*System Call*/
		printf( " Syscall Exiting..\n");
		if (return_type == 0 )	/*1 is error, 0 is success*/
		{
		printf( " sorted array: \n");
			for (i = 0; i< ARR_LEN; i++)
			{
							
				printf(" %d", sort_buf[i] );
				
			}
			
			printf("Sys call return status = %d\n", return_type);
			printf( "Process ID is: %d \n", syscall(20));	/*getpid()*/
			printf( "User ID is: %d \n", syscall(24));	/*getuid()*/

			/*Below code snippet sourced from web. Reference on header*/
			/*Sourced code Begin*/
			time_t T= time(NULL);
			struct  tm tm = *localtime(&T);
			 
			printf("Current Date is: %02d/%02d/%04d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
			printf("Current Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
			/*Sourced code End*/
			
		}
		else
		{	
			printf (" Error no is : %d\n", errno );
			perror("Error");
			
		}
		return 0;
	}
	else
		return 1;
}
