/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "sorter.h"

int main(int argc, char** argv) {

    printf("Sorter created!\n");   
    /*
    The parameters are passed this way:
    first the type of sort(H or Q) and then the rest of the 
    parameters that I'll just pass to heaposort or quicksort.
    Parameters: file input, start, end.
    */
    if(strcmp(argv[1], "H") == 0) {
        execlp("./heaposort", "./heapsort.c", argv[2], argv[3], argv[4], (char*) NULL);
    }
    else if(strcmp(argv[1], "Q") == 0) {
        execlp("./quicksort", "./quicksort.c", argv[2], argv[3], argv[4], (char*) NULL);
    }
    else {
        fprintf(stderr, "Sorter: What kind of algorithm is %s?\n", argv[1]);
    }

    return 0;
}