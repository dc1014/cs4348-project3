#ifndef CTS_FILESYSTEM_H
#define CTS_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string>
#include "FileSystem.h"

#define DATA_START 1024
#define BLOCK_OFFSET 511
#define FILE_TABLE_FIXED 64

using namespace std;

class CtsFileSystem : public FileSystem {
    private:
        int maxBuffer;
        int maxSize;
        int fileTablePosition;
        unsigned char bytes[131072];
    public:
        CtsFileSystem();
};

#endif
