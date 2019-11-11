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

    // Create appropriate number of sorters:
    int numberOfSorters;
    switch (coachID)
    {
        case 0:
            numberOfSorters = 1;
            char* newFifoFile = createFIFO("coach", coachID, 1);
            unsigned int start = 0;
            unsigned int end = numOfRecords-1;
            char startStr[UINT_STR_SIZE];
            char endStr[UINT_STR_SIZE];
            sprintf(startStr, "%u", start);
            sprintf(endStr, "%u", end);
            pid_t pid = fork();
            if(pid == 0) 
                execlp("./sorter", "./sorter", sortAlgorithm, inputFile, startStr, endStr,
                sortField, newFifoFile, (char*) NULL);

            // recordFIFO* newfifo = malloc(sizeof(recordFIFO));
            // newfifo->size = numOfRecords;
            // newfifo->fd = open(newFifoFile, O_RDONLY);
            
            // struct pollfd fdarray[1];
            // fdarray[0].fd = newfifo->fd;
            // fdarray[0].events = POLLIN;
            // int pollTime = poll(fdarray, 1, 300);
            // if(pollTime == 0) {
            //     fprintf(stderr, "Coach: fifo timed out.\n");
            //     exit(1);
            // }
            waitpid(pid, NULL, 0);
            break;
        case 1:
            numberOfSorters = 2;
            break;
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