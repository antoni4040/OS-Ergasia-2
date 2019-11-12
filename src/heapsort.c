/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "heapsort.h"

/*
For a given index in the record heap, make sure the record in
index maintains the heap property.
*/
void maxHeapify(recordHeap* givenHeap, unsigned int index) {
    Record** records = givenHeap->records;
    unsigned int sortField = givenHeap->sortField;
    
    unsigned int left = LEFT(index);
    unsigned int right = RIGHT(index);
    unsigned int largest;
    
    if(left < givenHeap->size && compareRecords(
        records[left], records[index], sortField) > 0) 
        largest = left;
    else
        largest = index;
    
    if(right < givenHeap->size && compareRecords(
        records[right], records[largest], sortField) > 0)
        largest = right;
    
    if(largest != index) {
        SWAP(records[index], records[largest]);
        maxHeapify(givenHeap, largest);
    }
}

/*
From a random array, build a max-heap.
*/
void buildMaxHeap(recordHeap* givenHeap) {
    int middle = givenHeap->length / 2;
    for(int i = middle - 1; i >= 0; i--) {
        maxHeapify(givenHeap, i);
    }
}

/*
Use buildMaxHeap and maxHeapify or sort an array of records.
*/
void heapsort(recordHeap* givenHeap) {
    buildMaxHeap(givenHeap);
    for(unsigned int i = givenHeap->length - 1; i >= 1; i--) {
        SWAP(givenHeap->records[0], givenHeap->records[i]);
        givenHeap->size -= 1;
        maxHeapify(givenHeap, 0);
    }
}

/*
Get input file and sort from given start to finish(including both start and finish).
*/
int main(int argc, char** argv) {
    char* fileName;
    unsigned int start;
    unsigned int end;
    unsigned int sortField;
    char* fifoFile;

    fileName = malloc(strlen(argv[1]) * sizeof(char) + 1);
    strcpy(fileName, argv[1]);
    start = strtoul(argv[2], NULL, 10);
    end = strtoul(argv[3], NULL, 10);
    sortField = strtoul(argv[4], NULL, 10);
    fifoFile = malloc(strlen(argv[5]) * sizeof(char) + 1);
    strcpy(fifoFile, argv[5]);

    printf("%u %u\n", start, end);
    
    recordHeap* newHeap = malloc(sizeof(recordHeap));
    newHeap->records = getRecords(fileName, start, end);
    newHeap->length = end - start + 1;
    newHeap->size = newHeap->length;
    newHeap->sortField = sortField;

    heapsort(newHeap);

    int fifofd = open(fifoFile, O_WRONLY | O_NONBLOCK);

    for(int i = 0; i < newHeap->length; i++) {
        write(fifofd, newHeap->records[i], sizeof(Record));
    }
    return 0;
}