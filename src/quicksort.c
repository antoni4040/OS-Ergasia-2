/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "quicksort.h"

/*
Partition given portion to items smaller and greater than the pivot.
*/
int partition(Record** records, int pivot, int right, unsigned int sortField) {
    Record* pivotElement = records[right];
    int below = pivot - 1;
    for(int above = pivot; above <= right - 1; above++) {
        if(compareRecords(records[above], pivotElement, sortField) <= 0) {
            below += 1;
            SWAP(records[below], records[above]);
        }
    }
    SWAP(records[below+1], records[right]);
    return below+1;
}

/*
Choose randomly and exchange pivot before partitioning.
*/
int randomizedPartition(Record** records, int pivot, int right, unsigned int sortField) {
    int randomIndex = rand() % (right-pivot) + pivot;
    SWAP(records[right], records[randomIndex]);
    return partition(records, pivot, right, sortField);
}

/*
Recursively partitions given array.
*/
void quicksortRecurse(Record** records, int pivot, int right, unsigned int sortField) {
    int partitionPoint;
    if(pivot < right) {
        partitionPoint = randomizedPartition(records, pivot, right, sortField);
        quicksortRecurse(records, pivot, partitionPoint - 1, sortField);
        quicksortRecurse(records, partitionPoint + 1, right, sortField);
    }
}

/*
Randomized quicksort. Best quicksort.
*/
void quicksort(Record** records, unsigned int size, unsigned int sortField) {
    srand(time(NULL));
    quicksortRecurse(records, 0, size - 1, sortField);
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

    Record** records = getRecords(fileName, start, end);

    quicksort(records, end-start+1, sortField);

    for(int i = 0; i < end-start+1; i++) {
        printRecord(records[i]);
    }
    return 0;
}