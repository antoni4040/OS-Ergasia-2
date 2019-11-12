/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "coach.h"

int main(int argc, char** argv) {
    if(argc != 6) {
        fprintf(stderr, "Coach: something's wrong with the command line parameters.\n");
        return 1;
    }

    printf("Coach: New coach created!\n");

    /*
    Get input file, number of records, coach ID(0, 1, 2, 3), sort algorithm
    and sort field from coordinator(parameters).
    */
    char* inputFile;
    unsigned int numOfRecords;
    int coachID;
    char* sortAlgorithm;
    char* sortField;
    inputFile = malloc(strlen(argv[1]) * sizeof(char) + 1);
    strcpy(inputFile, argv[1]);
    numOfRecords = strtoul(argv[2], NULL, 10);
    coachID = atoi(argv[3]);
    sortAlgorithm = argv[4];
    sortField = malloc(strlen(argv[5]) * sizeof(char) + 1);
    strcpy(sortField, argv[5]);

    recordFIFO* fifos[8];

    // Create coach data to easily pass to each case:
    coachData* coach = malloc(sizeof(coachData));
    coach->coachID = coachID;
    coach->inputFile = inputFile;
    coach->numOfRecords = numOfRecords;
    coach->sortAlgorithm = sortAlgorithm;
    coach->sortField = sortField;

    // Create appropriate number of sorters:
    int numberOfSorters;
    switch (coachID)
    {
        case 0: {
            caseOf1Sorter(coach);
            break;
        }
        case 1: {
            caseOf2Sorters(coach);
            break;
        }
        case 2:
            numberOfSorters = 4;
            break;
        case 3:
            numberOfSorters = 8;
            break;    
        default:
            fprintf(stderr, "Coach: something's wrong with the coach ID.\n");
            return 1;
    }
    printf("Coach: ready to die.\n");
    return 0;
}

/*
Redirect here when the coach must create only one sorter.
*/
void caseOf1Sorter(coachData* coach) {
    char* newFifoFile = createFIFO("coach", coach->coachID, 0);
    unsigned int start = 0;
    unsigned int end = coach->numOfRecords-1;
    char startStr[UINT_STR_SIZE];
    char endStr[UINT_STR_SIZE];
    sprintf(startStr, "%u", start);
    sprintf(endStr, "%u", end);
    pid_t pid = fork();

    // Child process:
    if(pid == 0) 
        execlp("./sorter", "./sorter", coach->sortAlgorithm, 
        coach->inputFile, startStr, endStr,
        coach->sortField, newFifoFile, (char*) NULL);

    // Parent process:
    recordFIFO* newfifo = malloc(sizeof(recordFIFO));
    newfifo->size = coach->numOfRecords;
    newfifo->fd = open(newFifoFile, O_RDONLY);

    // Setup somewhere to put incoming records:
    Record* records = malloc(sizeof(Record) * coach->numOfRecords);
    int recordsInCoach = 0;
    
    // Setup select:
    fd_set fds;
    
    FD_ZERO(&fds); 
    FD_SET(newfifo->fd, &fds);
    select(newfifo->fd + 1, &fds, NULL, NULL, NULL);

    // If the fifo has data:
    if (FD_ISSET(newfifo->fd, &fds)){
        // Read all records from fifo:
        while(recordsInCoach < coach->numOfRecords) {
            Record record;
            int result = read(newfifo->fd, &record, sizeof(Record));
            if(result > 0) {
                memcpy(&records[recordsInCoach], &record, sizeof(Record));
                recordsInCoach++;
            }
        }
    }

    for(int i = 0; i < recordsInCoach; i++) {
        printRecord(&records[i]);
    }

    int status;
    wait(&status);
    free(newfifo);
}

/*
Redirect here when the coach must create two sorters.
*/
void caseOf2Sorters(coachData* coach) {
    int numberOfSorters = 2; 
    for(int sorter = 0; sorter < numberOfSorters; sorter++) {
        char* newFifoFile = createFIFO("coach", coach->coachID, sorter);

        // Create first sorter for first half:
        unsigned int start = sorter * (coach->numOfRecords / 2);
        unsigned int end = start + (coach->numOfRecords / 2) - 1;
        char startStr[UINT_STR_SIZE];
        char endStr[UINT_STR_SIZE];
        sprintf(startStr, "%u", start);
        sprintf(endStr, "%u", end);
        pid_t pid = fork();
        if(pid == 0) 
            execlp("./sorter", "./sorter", coach->sortAlgorithm,
            coach->inputFile, startStr, endStr,
            coach->sortField, newFifoFile, (char*) NULL);
        }

    while (numberOfSorters > 0) {
        int status;
        wait(&status);
        numberOfSorters--;
    }
}