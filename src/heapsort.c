/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "heapsort.h"

/*
For a given index in the heap, make sure the item in
index maintains the heap property.
*/
void maxHeapify(heap* givenHeap, unsigned int index) {
    void** items = givenHeap->items;
    int (*compare)(void* itemA, void* itemB) = givenHeap->compare;
    
    unsigned int left = LEFT(index);
    unsigned int right = RIGHT(index);
    unsigned int largest;
    
    if(left <= givenHeap->size && compare(items[left], items[right]) > 0) 
        largest = left;
    else
        largest = index;
    
    if(right <= givenHeap->size && compare(items[right], items[largest]) > 0)
        largest = right;
    
    if(largest != index) {
        SWAP(items[index], items[largest]);
        maxHeapify(givenHeap, largest);
    }
}

/*
From a random array, build a max-heap.
*/
void buildMaxHeap(heap* givenHeap) {
    unsigned int middle = givenHeap->length / 2;
    for(unsigned int i = middle; i >= 0; i--) {
        maxHeapify(givenHeap, i);
    }
}

/*
Use buildMaxHeap and maxHeapify or sort an array of items.
*/
void heapsort(heap* givenHeap) {
    buildMaxHeap(givenHeap);
    for(unsigned int i = givenHeap->length - 1; i >= 1; i--) {
        SWAP(givenHeap->items[0], givenHeap->items[i]);
        givenHeap->size -= 1;
        maxHeapify(givenHeap, 0);
    }
}