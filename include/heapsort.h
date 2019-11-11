/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_HEAPSORT_H
#define ERGASIA_2_HEAPSORT_H

#define LEFT(i)     (2*i + 1)
#define RIGHT(i)    (2*i + 2)

/*
Based on the CLRS implementation that uses a separate size
and length(size will change during heapsort). Was going for a generic
design with a compare function pointer, but with the added
need for a field for sort it's really not worth it.
*/
#include <string.h>
#include <stdlib.h>
#include "files.h"

typedef struct {
    Record** records;
    unsigned int size;
    unsigned int length;
    unsigned int sortField;
} recordHeap;

void maxHeapify(recordHeap* givenHeap, unsigned int index);
void buildMaxHeap(recordHeap* givenHeap);
void heapsort(recordHeap* givenHeap);

#endif //ERGASIA_2_HEAPSORT_H