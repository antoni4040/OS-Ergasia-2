/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "coordinator.h"

int main(int argc, char** argv) {
    char* inputFile = NULL;
    char* checkIsNumber;
    int coachesToMake = 0;
    coachData coaches[4];

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
            int field = strtol(argv[i++], &checkIsNumber, 10);
            if (*checkIsNumber == 0) {  
                printf("No field given for coach %d, gonna use the first field...\n", coachesToMake + 1);
                field = 1;
            }

            coachData newCoach;
            newCoach.sortType = strcmp(argv[i], "-h") == 0 ? HEAP_SORT : QUICK_SORT;
            newCoach.field = field;
            coaches[coachesToMake] = newCoach;
            coachesToMake++;
        }
    }

    if(inputFile == NULL) {
        fprintf(stderr, "No input file given, nothing to do.\n");
        return 1;
    }

    printf("Coaches to make: %d\n", coachesToMake);
    return 0;
}