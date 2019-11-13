/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_FILES_H
#define ERGASIA_2_FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>

#define NORMAL_STRING 20
#define SHORT_STRING  6

#define SWAP(a, b) do {Record* x = a; a = b; b = x;} while(0) //Neat trick, wanted to use it for some time...

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

bool            checkFileExists(char* filename);
unsigned int    getNumberOfRecords(char* filename);
Record** 		getRecords(char* inputFile, unsigned int start, unsigned int end);
void 			writeRecords(char* inputFile, Record** records, unsigned int recordsCount, char* sortField);
char* 			createFIFO(char* name, int coachID, int numberOfSorters);
int 			compareRecords(Record* recordA, Record* recordB, unsigned int field);
void 			printRecord(Record* record);
#endif //ERGASIA_2_FILES_H