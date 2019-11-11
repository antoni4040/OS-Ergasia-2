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
#include <poll.h>

#include "files.h" 

#define UINT_STR_SIZE 11

/*
A helping struct to manage the multiple fifos that pass data.
*/
typedef struct {
    int fd;
    unsigned int size;
    void** records;
} recordFIFO;

#endif //ERGASIA_2_COACH_H  