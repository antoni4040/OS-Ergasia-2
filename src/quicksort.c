/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "quicksort.h"

/*
Partition given portion to items smaller and greater than the pivot.
*/
unsigned int partition(qSortItems* items, unsigned int pivot, unsigned int right) {
    void** pivotElement = items->items[right];
    long below = pivot - 1; //Can't be unsigned, it becomes -1 for the first iteration.
    for(unsigned int above = pivot; above <= right - 1; above++) {
        if(items->compare(items->items[above], pivotElement) <= 0) {
            below += 1;
            SWAP(items->items[below], items->items[above]);
        }
    }
    SWAP(items->items[below+1], items->items[right]);
    return below+1;
}

/*
Choose randomly and exchange pivot before partitioning.
*/
unsigned int randomizedPartition(qSortItems* items, unsigned int pivot, unsigned int right) {
    unsigned int randomIndex = rand() % (right-pivot) + pivot;
    SWAP(items->items[right], items->items[randomIndex]);
    return partition(items, pivot, right);
}

/*
Recursively partitions given array.
*/
void quicksortRecurse(qSortItems* items, unsigned int pivot, unsigned int right) {
    unsigned int partitionPoint;
    if(pivot < right) {
        partitionPoint = randomizedPartition(items, pivot, right);
        quicksortRecurse(items, pivot, partitionPoint - 1);
        quicksortRecurse(items, partitionPoint + 1, right);
    }
}

/*
Randomized quicksort. Best quicksort.
*/
void quicksort(qSortItems* items) {
    srand(time(NULL));
    quicksortRecurse(items, 0, items->size - 1);
}