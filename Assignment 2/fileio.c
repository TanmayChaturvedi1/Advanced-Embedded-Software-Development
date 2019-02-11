/*
 * Fileio.c file
 *
 *  Created on: Feb 8, 2019
 *      Author: Tanmay Chaturvedi

 		Description:
*		This is part of Assignment 2, Question 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define	MAX_SIZE	(20)
#define	INPUT_CHAR	(65)
#define FILE_NAME	("output.txt")

void main()

{
	printf(" My name is Tanmay Chaturvedi" );
	/*Creating a file and opening with write permission*/	
 	FILE* file_ptr = fopen( FILE_NAME, "w" );
	/*Change file permission to read and write*/
	chmod( FILE_NAME, 00400 | 00200);
	/*write a char to the file*/
	int open = fputc( INPUT_CHAR, file_ptr );	/*Writing 'A' (ASCII) 65*/
	/*Close file*/
 	fclose( file_ptr );

	fopen( FILE_NAME, "a+" );
	/*Dynamic memory allocation*/
	char* str_ptr= (char*)malloc( MAX_SIZE );	/*Dynamically allocating array of memory*/
	/*Null pointer check*/
	if(str_ptr)
	{	
		printf("Allocation Done");
	scanf( "%s",  str_ptr);	
	}
	/*Write input string to a file*/
	fputs( str_ptr, file_ptr );
	/*Flush file output*/
	fflush( file_ptr );
	fclose( file_ptr );
	/* Open file with read permission only*/
	fopen( FILE_NAME, "r" );
	/*read a char from file*/
	int read_char = fgetc( file_ptr );
	
	printf( "%d", read_char );
	/*read a string from file */
	char* read_str = fgets( str_ptr, MAX_SIZE, file_ptr );
	
	printf( "%s", str_ptr );
	fclose( file_ptr );
	free( str_ptr );

}
