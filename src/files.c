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
Record** getRecords(char* inputFile, unsigned int start, unsigned int end) {
    Record** records = malloc(sizeof(Record*) * (end-start+1)); //+1 for the end record
    unsigned int recordsCount = end-start+1;
    FILE* openFile = fopen(inputFile, "rb");
    fseek(openFile, start, SEEK_SET);
    for(unsigned int i = 0; i < recordsCount; i++) {
        Record* record = malloc(sizeof(Record));
        fread(record, sizeof(Record), 1, openFile);
        records[i] = record;
    }
    fclose(openFile);
    return records;
}

/*
Create a fifo for the coach-sorter relationship.
*/
char* createFIFO(char* name, int coachID, int numberOfSorters) {
    char* newName = malloc(strlen(name) + 20);
    char coachIDStr[10];
    char numberOfSortersStr[10];
    sprintf(coachIDStr, "%d", coachID);
    sprintf(numberOfSortersStr, "%d", numberOfSorters);
    strcpy(newName, name);
    strcat(newName, coachIDStr);
    strcat(newName, numberOfSortersStr);

    int retVal = mkfifo(newName, 0666);

    if((retVal == -1) && (errno != EEXIST)) {
        fprintf(stderr, "Something went wrong while creating the fifo!\n");
        exit(1);
    }
    return newName;
}

/*
Compare two given records based on the given struct field.
*/
int compareRecords(Record* recordA, Record* recordB, unsigned int field) {
    switch (field)
    {
    case 0:
        return (int)(recordA->custid - recordB->custid);
    case 1:
        return strcmp(recordA->FirstName, recordB->FirstName);
    case 2:
        return strcmp(recordA->LastName, recordB->LastName);
    case 3:
        return strcmp(recordA->Street, recordB->Street);
    case 4:
        return (int)(recordA->HouseID - recordB->HouseID);
    case 5:
        return strcmp(recordA->City, recordB->City);
    case 6:
        return strcmp(recordA->postcode, recordB->postcode);
    case 7:
        return (recordA->amount != recordB->amount ? (  // float to int might cause problems
            recordA->amount < recordB->amount ? -1 : 1
        ) : 0);
    default:
        break;
    }
    fprintf(stderr, "Something terrible has happened!\n");
    return 0;
}

/*
A helping function to print the record data.
*/
void printRecord(Record* record) {
    printf("%ld %s %s %s %d %s %s %f\n", record->custid,
        record->FirstName, record->LastName, record->Street,
        record->HouseID, record->City, record->postcode, record->amount);
}