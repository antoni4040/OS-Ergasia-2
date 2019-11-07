/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_COORDINATOR_H
#define ERGASIA_2_COORDINATOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HEAP_SORT  0
#define QUICK_SORT 1
#define MAX_FIELD_NUMBER 8

typedef struct coachData {
    int sortType: 1;
    int field:    4;
} coachData;

#endif //ERGASIA_2_COORDINATOR_H