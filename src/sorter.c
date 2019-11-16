/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "sorter.h"

int main(int argc, char** argv) {
    /*
    The parameters are passed this way:
    first the type of sort(H or Q) and then the rest of the 
    parameters that I'll just pass to heapsort or quicksort.
    Parameters: file input, start, end, field, fifo.
    */
    if(strcmp(argv[1], "H") == 0) {
        execlp("./heapsort", "./heapsort", argv[2], argv[3], argv[4],
            argv[5], argv[6], (char*) NULL);
    }
    else if(strcmp(argv[1], "Q") == 0) {
        execlp("./quicksort", "./quicksort", argv[2], argv[3], argv[4],
            argv[5], argv[6], (char*) NULL);
    }
    else {
        fprintf(stderr, "Sorter: What kind of algorithm is %s?\n", argv[1]);
    }

    return 0;
}