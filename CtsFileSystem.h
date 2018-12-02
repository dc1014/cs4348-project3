#ifndef CTS_FILESYSTEM_H
#define CTS_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string>
#include "FileSystem.h"

using namespace std;

class CtsFileSystem : public FileSystem {
    public:
        CtsFileSystem();
        void readFile(char *, char *);
        void displayFile(char *);
        void deleteFile(char *);
};

#endif
