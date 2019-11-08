/* 
Created by Antonis Karvelas, sdi1600060. K22, Project 2.
*/

#include "files.h"

/*
Simple check for whether given file exists or not using stat.
*/
bool checkFileExists(char* filename) {
    struct stat fileExists;
    if (!(stat(filename, &fileExists) == 0 && S_ISREG(fileExists.st_mode)))
    {
        return false;
    }
    return true;
}

/*
Open given filename and calculate number of records.
*/
unsigned int getNumberOfRecords(char* filename) {
    FILE* openFile = fopen (filename, "rb");
    if(openFile == NULL) {
        fprintf(stderr, "Can't open file. Maybe an introvert?\n");
        return 0;
    }

    fseek(openFile, 0, SEEK_END);
    long dataSize = ftell(openFile);
    rewind(openFile);
    unsigned int numOfrecords = (unsigned int)dataSize/sizeof(Record);

    fclose(openFile);
    return numOfrecords;
}

/*
Get records from input file and make an array.
*/
void** getItems(char* inputFile, unsigned int start, unsigned int end) {
    void** items = malloc(sizeof(void*) * (end-start+1)); //+1 for the end record
    unsigned int itemsCount = (end-start+1) * sizeof(Record);
    FILE* openFile = fopen(inputFile, "rb");
    fseek(openFile, start, SEEK_SET);
    for(unsigned int i = 0; i < itemsCount; i++) {
        Record* record = malloc(sizeof(Record));
        fread(record, sizeof(Record), 1, openFile);
        items[i] = (void*)record;
    }
    fclose(openFile);
    return items;
}