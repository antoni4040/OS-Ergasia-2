/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_FILES_H
#define ERGASIA_2_FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

#define NORMAL_STRING 20
#define SHORT_STRING  6

typedef struct{
	long  	custid;
	char 	FirstName[NORMAL_STRING];
	char 	LastName[NORMAL_STRING];
	char	Street[NORMAL_STRING];
	int 	HouseID;
	char	City[NORMAL_STRING];
	char	postcode[SHORT_STRING];
	float  	amount;
} Record;

bool            checkInputFileExists(char* filename);
unsigned int    getNumberOfRecords(char* filename);

#endif //ERGASIA_2_FILES_H