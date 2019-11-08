/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_QUICKSORT_H
#define ERGASIA_2_QUICKSORT_H

#include <stdlib.h>
#include <time.h> 

#define SWAP(a, b) do {void* x = a; a = b; b = x;} while(0) //Neat trick, wanted to use it for some time...

/*
Using the CLRS implementation of quicksort. Just for convenience,
I made a struct to hold both the items to sort and the compare function.
*/
typedef struct {
    void** items;
    unsigned int size;
    int (*compare)(void* itemA, void* itemB);
} qSortItems;

unsigned int    partition(qSortItems* items, unsigned int pivot, unsigned int right);
unsigned int    randomizedPartition(qSortItems* items, unsigned int pivot, unsigned int right);
void            quicksortRecurse(qSortItems* items, unsigned int pivot, unsigned int right);
void            quicksort(qSortItems* items);

#endif //ERGASIA_2_HEAPSORT_H