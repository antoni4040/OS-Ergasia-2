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

    // Create coach data to easily pass to each case:
    coachData* coach = malloc(sizeof(coachData));
    coach->coachID = coachID;
    coach->inputFile = inputFile;
    coach->numOfRecords = numOfRecords;
    coach->sortAlgorithm = sortAlgorithm;
    coach->sortField = sortField;

    // Create appropriate number of sorters:
    switch (coachID)
    {
        case 0: {
            caseOf1Sorter(coach);
            break;
        }
        case 1: {
            int portions[2] = {1, 1};
            caseOfNSorters(coach, 2, 2, portions);
            break;
        }
        case 2: {
            int portions[4] = {1, 1, 2, 4};
            caseOfNSorters(coach, 4, 8, portions);
            break;
        }
        case 3: {
            int portions[8] = {1, 1, 1, 1, 2, 2, 4, 4};
            caseOfNSorters(coach, 4, 16, portions);
            break;    
        }
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
    if(pid == 0) {
        execlp("./sorter", "./sorter", coach->sortAlgorithm, 
        coach->inputFile, startStr, endStr,
        coach->sortField, newFifoFile, (char*) NULL);
    }

    // Parent process:
    recordFIFO* newfifo = malloc(sizeof(recordFIFO));
    newfifo->size = coach->numOfRecords;
    newfifo->fd = open(newFifoFile, O_RDONLY);

    // Setup somewhere to put incoming records:
    Record** records = malloc(sizeof(Record*) * coach->numOfRecords);
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
            Record* record = malloc(sizeof(Record));
            int result = read(newfifo->fd, record, sizeof(Record));
            if(result > 0) {
                records[recordsInCoach] = record;
                recordsInCoach++;
            }
            else 
                free(record);
        }
    }

    //Write records to text file in ASCII:
    writeRecords(coach->inputFile, records, coach->numOfRecords, coach->sortField);

    // Wait for child process to finish:
    int status;
    wait(&status);
    free(newfifo);

    // Free allocated memory for records:
    for(int i = 0; i < coach->numOfRecords; i++) {
        free(records[i]);
    }
    free(records);
}

/*
Redirect here when the coach must create two sorters.
*/
void caseOfNSorters(coachData* coach, int numberOfSorters, int divider, int* portions) {
    recordFIFO* fifos[numberOfSorters];

    // Setup somewhere to put incoming records:
    Record** records[numberOfSorters];
    int recordsInCoach[numberOfSorters];
    int totalRecordsToWait[numberOfSorters];

    int maxfd = 0;
    unsigned int last = 0;
    for(int sorter = 0; sorter < numberOfSorters; sorter++) {
        char* newFifoFile = createFIFO("coach", coach->coachID, sorter);

        // Create first sorter for first half:
        unsigned int start = last;
        unsigned int end = start + (coach->numOfRecords / divider)*portions[sorter] - 1;
        last = end + 1;
        // Remaining records after divisions go to the last segment:
        if(sorter == numberOfSorters - 1) 
            end = coach->numOfRecords - 1;
        char startStr[UINT_STR_SIZE];
        char endStr[UINT_STR_SIZE];
        sprintf(startStr, "%u", start);
        sprintf(endStr, "%u", end);
        pid_t pid = fork();
        if(pid == 0) {
            execlp("./sorter", "./sorter", coach->sortAlgorithm,
            coach->inputFile, startStr, endStr,
            coach->sortField, newFifoFile, (char*) NULL);
        }
        
        // Parent process:
        recordFIFO* newfifo = malloc(sizeof(recordFIFO));
        newfifo->size = coach->numOfRecords;
        newfifo->fd = open(newFifoFile, O_RDONLY);
        fifos[sorter] = newfifo;

        maxfd = newfifo->fd > maxfd ? newfifo->fd : maxfd;

        totalRecordsToWait[sorter] = end - start + 1;
        records[sorter] = malloc(sizeof(Record*) * (end - start + 1));
        recordsInCoach[sorter] = 0;
    }

    // Setup select:
    fd_set fds;
    
    int sortersToReceive = numberOfSorters;
    while(sortersToReceive > 0) {
        FD_ZERO(&fds); 
        for(int sorter = 0; sorter < numberOfSorters; sorter++) {
            FD_SET(fifos[sorter]->fd, &fds);
        }

        select(maxfd + 1, &fds, NULL, NULL, NULL);

        for(int sorter = 0; sorter < numberOfSorters; sorter++) {
            // If the fifo has data:
            if (FD_ISSET(fifos[sorter]->fd, &fds)){
                // Read all records from fifo:
                while(recordsInCoach[sorter] < totalRecordsToWait[sorter]) {
                    Record* record = malloc(sizeof(Record));
                    int result = read(fifos[sorter]->fd, record, sizeof(Record));
                    if(result > 0) {
                        records[sorter][recordsInCoach[sorter]] = record;
                        recordsInCoach[sorter]++;
                    }
                    else {
                        free(record);
                        break;
                    }
                }
                if(totalRecordsToWait[sorter] == recordsInCoach[sorter])
                    sortersToReceive--;
            }
        }
    }

    for(int sorter = 0; sorter < numberOfSorters; sorter++) {
        for(int i = 0; i < recordsInCoach[sorter]; i++) {
            printRecord(records[sorter][i]);
        }
        printf("\n\n");
    }

    // Wait for children to finish:
    while (numberOfSorters > 0) {
        int status;
        wait(&status);
        numberOfSorters--;
    }
}