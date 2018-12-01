#ifndef INDEXED_FILESYSTEM_H
#define INDEXED_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string>
#include <vector>
#include <cstdlib>
#include "FileSystem.h"

#define DATA_START 1024
#define BLOCK_OFFSET 511
#define FILE_TABLE_FIXED 64

using namespace std;

class IndexedFileSystem : public FileSystem {
    public:
        IndexedFileSystem();
        vector<int> claimBlocks(int);
        int* findFileBlocks(char *);
        void deleteFile(char *);
        void displayFile(char *);
        void readFile(char *, char *);
        void writeFile(char *, char *);
        int writeToSystem(char *, vector<int>);
        void writeToTable(char *, vector<int>, int);
};

#endif