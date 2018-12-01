#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string>

#define DATA_START 1024
#define BLOCK_OFFSET 511
#define FILE_TABLE_FIXED 64

using namespace std;

class FileSystem {
    protected:
        int maxBuffer;
        int maxSize;
        int fileTablePosition;
        unsigned char bytes[131072];
    public:
        FileSystem();
        int* claimBlocks(int);
        int* findFileBlocks(char *);
        void deleteFile(char *);
        void displayFile(char *);
        void printBitmap();
        void printBlock(int);
        void printTable();
        virtual void readFile(char *, char *);
        void writeFile(char *, char *);
        int writeToSystem(char *, int*);
        void writeToTable(char *, int*, int);
        int blockStart(int);
};

#endif
