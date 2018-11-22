#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <fstream>

#define DATA_START 1024
#define BLOCK_OFFSET 511

using namespace std;

class FileSystem {
    private:
        int maxBuffer;
        int maxSize;
        int fileTablePosition;
        unsigned char bytes[131072];
    public:
        FileSystem();
        int* findBlocks(int);
        void printBitmap();
        void printTable();
        void readFile(char *, char *);
        void writeToTable(char *, int*);
};

#endif
