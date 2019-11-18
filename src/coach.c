/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "coach.h"

static int signalsReceived = 0;

int main(int argc, char** argv) {
    static struct sigaction action;
	action.sa_handler = handleSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;      
    sigaction(SIGUSR2, &action, NULL);
        
    if(argc != 7) {
        fprintf(stderr, "Coach: something's wrong with the command line parameters.\n");
        return 1;
    }

    struct tms calculateTime;
    double ticspersec = (double) sysconf(_SC_CLK_TCK);
    double startTime = (double)times(&calculateTime);

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
    sortAlgorithm = malloc(strlen(argv[4]) * sizeof(char) + 1);
    strcpy(sortAlgorithm, argv[4]);
    sortField = malloc(strlen(argv[5]) * sizeof(char) + 1);
    strcpy(sortField, argv[5]);
    coordinatorFifo = malloc(strlen(argv[6]) * sizeof(char) + 1);
    strcpy(coordinatorFifo, argv[6]);

    // Create coach data to easily pass to each case:
    coachData* coach = malloc(sizeof(coachData));
    coach->coachID = coachID;
    coach->inputFile = inputFile;
    coach->numOfRecords = numOfRecords;
    coach->sortAlgorithm = sortAlgorithm;
    coach->sortField = sortField;

    int numOfSorters;

    switch (coachID)
    {
        case 0: {
            numOfSorters = 1;
            coach->durations = malloc(sizeof(double));
            int portions[1] = {1};
            caseOfNSorters(coach, 1, 1, portions);
            break;
        }
        case 1: {
            numOfSorters = 2;
            coach->durations = malloc(sizeof(double) * 2);
            int portions[2] = {1, 1};
            caseOfNSorters(coach, 2, 2, portions);
            break;
        }
        case 2: {
            numOfSorters = 4;
            coach->durations = malloc(sizeof(double) * 4);
            int portions[4] = {1, 1, 2, 4};
            caseOfNSorters(coach, 4, 8, portions);
            break;
        }
        case 3: {
            numOfSorters = 8;
            coach->durations = malloc(sizeof(double) * 8);
            int portions[8] = {1, 1, 1, 1, 2, 2, 4, 4};
            caseOfNSorters(coach, 8, 16, portions);
            break;    
        }
        default:
            fprintf(stderr, "Coach: something's wrong with the coach ID.\n");
            return 1;
    }

    double min = minDuration(coach->durations, numOfSorters);
    double max = maxDuration(coach->durations, numOfSorters);
    double average = averageDuration(coach->durations, numOfSorters);

    double endTime = (double)times(&calculateTime);
    double duration = (endTime - startTime) / ticspersec;

    // Pass statistics to coordinator:
    int fifofd = open(coordinatorFifo, O_WRONLY);
    write(fifofd, &min, sizeof(double));
    write(fifofd, &max, sizeof(double));
    write(fifofd, &average, sizeof(double));
    write(fifofd, &duration, sizeof(double));
    write(fifofd, &signalsReceived, sizeof(int));
    close(fifofd);

    free(coach->inputFile);
    free(coach->durations);
    free(coach);

    free(sortAlgorithm);
    free(sortField);
    free(coordinatorFifo);

    return 0;
}

/*
I managed to abstract it enough to only need one function to do the job.
It's a mess though. Would refactor heavily in a production environment.
*/
void caseOfNSorters(coachData* coach, int numberOfSorters, int divider, int* portions) {
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGUSR2);

    recordFIFO* fifos[numberOfSorters];

    // Setup somewhere to put incoming records:
    Record** records[numberOfSorters];
    int recordsInSorter[numberOfSorters];
    int totalRecordsToWait[numberOfSorters];

    // Need to mask this part cause for some God-damn fucking awful
    // reason, SIGUSR2 breaks something if it's received.
    sigprocmask(SIG_BLOCK, &signal_set, NULL);

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
            if(strcmp(coach->sortAlgorithm, "H") == 0) {
            execlp("./heapsort", "./heapsort", coach->inputFile, startStr, endStr,
            coach->sortField, newFifoFile, (char*)NULL);
            }
            else if(strcmp(coach->sortAlgorithm, "Q") == 0) {
                execlp("./quicksort", "./quicksort", coach->inputFile, startStr, endStr,
                coach->sortField, newFifoFile, (char*)NULL);
            }
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

    sigprocmask(SIG_UNBLOCK, &signal_set, NULL);

    
    int sortersToReceive = numberOfSorters;
    while(sortersToReceive > 0) {
        // Setup select:
        fd_set fds;
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

                    if(totalRecordsToWait[sorter] == recordsInSorter[sorter]) {
                        while(read(fifos[sorter]->fd, &(coach->durations[sorter]), sizeof(double)));
                        sortersToReceive--;
                        close(fifos[sorter]->fd);
                    }
                }
            }
        }
    }

    // Merge records:
    Record** recordsMerged = records[0];
    int recordsMergedCount = recordsInSorter[0];
    if(numberOfSorters > 1) {
        for(int sorter = 1; sorter < numberOfSorters; sorter++) {
            recordsMerged = mergeRecords(recordsMerged, records[sorter],
                recordsMergedCount, recordsInSorter[sorter], atoi(coach->sortField));
            free(records[sorter-1]);
            recordsMergedCount += recordsInSorter[sorter];
        }
        free(records[numberOfSorters-1]);
    }

    // Write records to file:
    writeRecords(coach->inputFile, recordsMerged, recordsMergedCount, coach->sortField);

    for(int i = 0; i < recordsMergedCount; i++) {
        free(recordsMerged[i]);
    }
    free(recordsMerged);

    // Delete fifos:
    for(int i = 0; i < numberOfSorters; i++) {
        remove(fifos[i]->filename);
        free(fifos[i]->filename);
        free(fifos[i]);
    }

    pid_t pid;
    int status;
    while((pid=wait(&status)) > 0);
}

/*
Handle the signal. This is not the right way to do things, but the exercise
doesn't ask for anything more.
*/
void handleSignal(int signum) {
    signalsReceived++;
}
