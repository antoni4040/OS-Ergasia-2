/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_HEAPSORT_H
#define ERGASIA_2_HEAPSORT_H

#define LEFT(i)     (2*i + 1)
#define RIGHT(i)    (2*i + 2)
#define SWAP(a, b) do {void* x = a; a = b; b = x;} while(0) //Neat trick, wanted to use it for some time...

/*
Based on the CLRS implementation that uses a separate size
and length(size will change during heapsort). I've also added
a function pointer to a compare function has to return a negative if
itemA < itemB, a positive if itemA > itemB and 0 if itemA = itemB.
*/
typedef struct {
    void** items;
    unsigned int size;
    unsigned int length;
    int (*compare)(void* itemA, void* itemB);
} heap;

void maxHeapify(heap* givenHeap, unsigned int index);
void buildMaxHeap(heap* givenHeap);
void heapsort(heap* givenHeap);

#endif //ERGASIA_2_HEAPSORT_H