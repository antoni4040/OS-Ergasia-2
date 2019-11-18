/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "coordinator.h"

int main(int argc, char** argv) {
    char* inputFile = NULL;
    char* checkIsNumber;
    int coachesToMake = 0;
    coachData coaches[4];
    double durations[4];

    struct tms calculateTime;
    double ticspersec = (double) sysconf(_SC_CLK_TCK);
    double startTime = (double)times(&calculateTime);

    if(argc == 1)   // Abort if no command line arguments given:
    {
        fprintf(stderr, "Hey little guy, you didn't give me any command line arguments...\n");
        return 1;
    }

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-f") == 0) {
            i++;
            inputFile = (char*)malloc(strlen(argv[i])*sizeof(char) + 1);
            strcpy(inputFile, argv[i]);
        }
        else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-q") == 0) {
            int field = strtol(argv[i+1], &checkIsNumber, 10);
            if (*checkIsNumber != '\0') {  
                printf("Coordinator: No field given for coach %d, gonna use the first field...\n", coachesToMake + 1);
                field = 1;
            }
            coachData newCoach;
            newCoach.sortType = strcmp(argv[i], "-h") == 0 ? HEAP_SORT : QUICK_SORT;
            newCoach.field = field;
            coaches[coachesToMake] = newCoach;
            coachesToMake++;
            i++;
        }
    }

    if(inputFile == NULL) {
        fprintf(stderr, "Coordinator: Bert watches in horror as Ernie didn't give an input file.\n");
        return 1;
    }

    if(checkFileExists(inputFile) == false) {
        fprintf(stderr, "Coordinator: WHERE IS THE FILE LEBOWSKI?\n");
        return 1;
    }

    printf("Coordinator: Coaches to make: %d\n", coachesToMake);

    unsigned int numOfRecords = getNumberOfRecords(inputFile);

    for(int i = 0; i < coachesToMake; i++) {
        coachData currectCoach = coaches[i];

        char* newCoordinatorFifo = createFIFO("coordinator", 0, i);
        coaches[i].fifo = newCoordinatorFifo;
        
        // Pass input file, number of records, coach ID(0, 1, 2, 3),
        // sort algorithm, sort field and coordinator fifo.
        char numOfRecordsStr[10];
        sprintf(numOfRecordsStr, "%u", numOfRecords);
        char coachIDStr[2];
        sprintf(coachIDStr, "%d", i);
        char sortAlgorithmStr[2];
        if(currectCoach.sortType == HEAP_SORT)
            strcpy(sortAlgorithmStr, "H");
        else
            strcpy(sortAlgorithmStr, "Q");
        char sortFieldStr[2];
        sprintf(sortFieldStr, "%d", currectCoach.field);

        // printf("%s %s %s %s %d\n", inputFile, numOfRecordsStr, coachIDStr, sortAlgorithmStr, currectCoach.field);
                
        if(fork() == 0) {
            execlp("./coach", "./coach",
                inputFile, numOfRecordsStr, 
                coachIDStr, sortAlgorithmStr,
                sortFieldStr, newCoordinatorFifo, (char*) NULL);
        }
    }

    // Open fifos:
    int maxfd = 0;
    for(int i = 0; i < coachesToMake; i++) {
        coaches[i].fifofd = open(coaches[i].fifo, O_RDONLY);
        maxfd = coaches[i].fifofd > maxfd ? coaches[i].fifofd : maxfd;
    }

    // Setup select:
    fd_set fds;
    int coachesToReceive = coachesToMake;
    while(coachesToReceive > 0) {
        FD_ZERO(&fds); 
        for(int i = 0; i < coachesToMake; i++) {
            FD_SET(coaches[i].fifofd, &fds);
        }

        select(maxfd + 1, &fds, NULL, NULL, NULL);

        for(int coach = 0; coach < coachesToMake; coach++) {
            // If the fifo has data:
            if (FD_ISSET(coaches[coach].fifofd, &fds)) { 
                read(coaches[coach].fifofd, &(coaches[coach].minTime), sizeof(double));
                read(coaches[coach].fifofd, &(coaches[coach].maxTime), sizeof(double));
                read(coaches[coach].fifofd, &(coaches[coach].avgTime), sizeof(double));
                read(coaches[coach].fifofd, &(durations[coach]), sizeof(double));
                read(coaches[coach].fifofd, &(coaches[coach].signals), sizeof(int));
                coachesToReceive--;
                close(coaches[coach].fifofd);
            }
        }
    }

    // Print sorters MIN, MAX, AVG times and signals received.
    for(int i = 0; i < coachesToMake; i++) {
        coachData coach = coaches[i];
        printf("Coach %d: Sorter: MIN: %lf, MAX: %lf, AVG: %lf, SIGNALS: %d\n", i,
            coach.minTime, coach.maxTime, coach.avgTime, coach.signals);
        free(coach.fifo);
    }

    double min = minDuration(durations, coachesToMake);
    double max = maxDuration(durations, coachesToMake);
    double average = averageDuration(durations, coachesToMake); 
    printf("Coordinator: MIN: %lf, MAX: %lf, AVG: %lf\n", min, max, average);

    double endTime = (double)times(&calculateTime);
    double duration = (endTime - startTime) / ticspersec;
    printf("Coordinator: Turnaround Time: %lf\n", duration);

    pid_t pid;
    int status;
    while((pid=wait(&status)) > 0);   //Wait for all child processes to finish.
    free(inputFile);
    return 0;
}