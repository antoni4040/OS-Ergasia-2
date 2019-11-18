/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#ifndef ERGASIA_2_COACH_H
#define ERGASIA_2_COACH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/times.h>
#include <signal.h>

#include "files.h" 

#define UINT_STR_SIZE 11

typedef struct {
    int coachID;
    char* inputFile;
    char* sortAlgorithm;
    char* sortField;
    unsigned int numOfRecords;
    double* durations;
} coachData;

/*
A helping struct to manage the multiple fifos that pass data.
*/
typedef struct {
    int fd;
    char* filename;
    unsigned int size;
    void** records;
} recordFIFO;

void caseOfNSorters(coachData* coach, int numberOfSorters, int divider, int* portions);
void handleSignal(int signum);
#endif //ERGASIA_2_COACH_H  