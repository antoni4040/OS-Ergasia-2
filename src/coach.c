/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "coach.h"

int signalsReceived = 0;

int main(int argc, char** argv) {
    if(argc != 7) {
        fprintf(stderr, "Coach: something's wrong with the command line parameters.\n");
        return 1;
    }

    /*
    Get input file, number of records, coach ID(0, 1, 2, 3), sort algorithm
    and sort field from coordinator(parameters).
    */
    char* inputFile;
    unsigned int numOfRecords;
    int coachID;
    char* sortAlgorithm;
    char* sortField;
    char* coordinatorFifo;
    inputFile = malloc(strlen(argv[1]) * sizeof(char) + 1);
    strcpy(inputFile, argv[1]);
    numOfRecords = strtoul(argv[2], NULL, 10);
    coachID = atoi(argv[3]);
    sortAlgorithm = argv[4];
    sortField = malloc(strlen(argv[5]) * sizeof(char) + 1);
    strcpy(sortField, argv[5]);
    coordinatorFifo = malloc(strlen(argv[6]) * sizeof(char) + 1);
    strcpy(coordinatorFifo, argv[6]);

    printf("Coach %d: created!\n", coachID);
    printf("%s\n", coordinatorFifo);

    // Create coach data to easily pass to each case:
    coachData* coach = malloc(sizeof(coachData));
    coach->coachID = coachID;
    coach->inputFile = inputFile;
    coach->numOfRecords = numOfRecords;
    coach->sortAlgorithm = sortAlgorithm;
    coach->sortField = sortField;

    signal(SIGUSR2, handleSignal);
    // Create appropriate number of sorters:
    switch (coachID)
    {
        case 0: {
            coach->durations = malloc(sizeof(double));
            caseOf1Sorter(coach);
            break;
        }
        case 1: {
            coach->durations = malloc(sizeof(double) * 2);
            int portions[2] = {1, 1};
            caseOfNSorters(coach, 2, 2, portions);
            break;
        }
        case 2: {
            coach->durations = malloc(sizeof(double) * 4);
            int portions[4] = {1, 1, 2, 4};
            caseOfNSorters(coach, 4, 8, portions);
            break;
        }
        case 3: {
            coach->durations = malloc(sizeof(double) * 8);
            int portions[8] = {1, 1, 1, 1, 2, 2, 4, 4};
            caseOfNSorters(coach, 8, 16, portions);
            break;    
        }
        default:
            fprintf(stderr, "Coach: something's wrong with the coach ID.\n");
            return 1;
    }
    free(coach->inputFile);
    free(coach->durations);
    free(coach);
    printf("Coach %d: ready to die. Signals: %d\n", coachID, signalsReceived);
    return 0;
}

/*
Redirect here when the coach must create only one sorter.
*/
void caseOf1Sorter(coachData* coach) {
    signal(SIGUSR2, handleSignal);
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
    int recordsInSorter = 0;
    
    // Setup select:
    fd_set fds;
    
    FD_ZERO(&fds); 
    FD_SET(newfifo->fd, &fds);
    select(newfifo->fd + 1, &fds, NULL, NULL, NULL);

    // If the fifo has data:
    if (FD_ISSET(newfifo->fd, &fds)){
        // Read all records from fifo:
        while(recordsInSorter < coach->numOfRecords) {
            Record* record = malloc(sizeof(Record));
            int result = read(newfifo->fd, record, sizeof(Record));
            if(result > 0) {
                records[recordsInSorter] = record;
                recordsInSorter++;
            }
            else 
                free(record);
        }
    }
    
    read(newfifo->fd, &(coach->durations[0]), sizeof(double));

    // Wait for child process to finish:
    int status;
    wait(&status);

    printf("Coach %d: sorter 0: %lf\n", coach->coachID, coach->durations[0]);
    //Write records to text file in ASCII:
    writeRecords(coach->inputFile, records, coach->numOfRecords, coach->sortField);

    remove(newFifoFile);
    free(newFifoFile);
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
    signal(SIGUSR2, handleSignal);
    recordFIFO* fifos[numberOfSorters];

    // Setup somewhere to put incoming records:
    Record** records[numberOfSorters];
    int recordsInSorter[numberOfSorters];
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
        newfifo->filename = newFifoFile;
        fifos[sorter] = newfifo;

        maxfd = newfifo->fd > maxfd ? newfifo->fd : maxfd;

        totalRecordsToWait[sorter] = end - start + 1;
        records[sorter] = malloc(sizeof(Record*) * (end - start + 1));
        recordsInSorter[sorter] = 0;
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
                while(recordsInSorter[sorter] < totalRecordsToWait[sorter]) {
                    Record* record = malloc(sizeof(Record));
                    int result = read(fifos[sorter]->fd, record, sizeof(Record));
                    if(result > 0) {
                        records[sorter][recordsInSorter[sorter]] = record;
                        recordsInSorter[sorter]++;
                    }
                    else {
                        free(record);
                        break;
                    }
                }
                if(totalRecordsToWait[sorter] == recordsInSorter[sorter]) {
                    read(fifos[sorter]->fd, &(coach->durations[sorter]), sizeof(double));
                    sortersToReceive--;
                }
            }
        }
    }

    // Merge records:
    Record** recordsMerged = records[0];
    int recordsMergedCount = recordsInSorter[0];
    for(int sorter = 1; sorter < numberOfSorters; sorter++) {
        recordsMerged = mergeRecords(recordsMerged, records[sorter],
            recordsMergedCount, recordsInSorter[sorter], atoi(coach->sortField));
        free(records[sorter-1]);
        recordsMergedCount += recordsInSorter[sorter];
    }
    free(records[numberOfSorters-1]);

    // Write records to file:
    writeRecords(coach->inputFile, recordsMerged, recordsMergedCount, coach->sortField);

    // Print time and delete fifos:
    for(int i = 0; i < numberOfSorters; i++) {
        printf("Coach %d: sorter %d: %lf\n", coach->coachID, i, coach->durations[i]);
        remove(fifos[i]->filename);
    }

    // Wait for children to finish:
    while (numberOfSorters > 0) {
        int status;
        wait(&status);
        numberOfSorters--;
    }
}

/*
Handle the signal. This is not the right way to do things, but the exercise
doesn't ask for anything more.
*/
void handleSignal(int signum) {
    signal(SIGUSR2, handleSignal);
    signalsReceived++;
}
