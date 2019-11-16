/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_QUICKSORT_H
#define ERGASIA_2_QUICKSORT_H

#include <stdlib.h>
#include <time.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "files.h"
#include <signal.h>


/*
Using the CLRS implementation of quicksort. Just for convenience,
I made a struct to hold both the records to sort and the size.
*/

int     partition(Record** records, int pivot, int right, unsigned int sortField);
int     randomizedPartition(Record** records, int pivot, int right, unsigned int sortField);
void    quicksortRecurse(Record** records, int pivot, int right, unsigned int sortField);
void    quicksort(Record** records, unsigned int size, unsigned int sortField);

#endif //ERGASIA_2_HEAPSORT_H