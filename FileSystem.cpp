#include "FileSystem.h"

FileSystem::FileSystem() {
    maxBuffer = 512;
    maxSize = 512 * 10;
    fileTablePosition = 0;
    for (int i = 0; i < 131072; i++) {
        bytes[i] = '0';
    }
    bytes[512] = '1';
    bytes[513] = '1';
}

// abstract
void FileSystem::readFile(char* fileName, char* targetName) {
    ifstream is (fileName, ifstream::binary);
    int* blocks;

    if (is) {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();

        if (length > maxSize) {
            cout << "File too big, skipping" << endl;
            is.close();
        }
        else {
            is.seekg (0, is.beg);

            char * buffer = new char [length];

            // read data as a block:
            is.read (buffer,length);

            blocks = findBlocks(length);

            if (blocks[0] == 0) {
                cout << "not enough space" << endl;
            }

            else {

                if (is) {
                    writeToSystem(buffer, blocks);
                    writeToTable(targetName, blocks);
                    cout << "all characters read successfully.";
                }
                else {
                  cout << endl << "error: only " << is.gcount() << " could be read";
                }
            }

            is.close();
            // ...buffer contains the entire file...

            delete[] buffer;

        }
    }
}

// abstract
int* FileSystem::findBlocks(int fileSize) {
    static int blocks[10] = { 0 };
    int counter = 0;
    int tempSize = fileSize;
    bool attempt = false;

    for(int i = 514; i < 768 && tempSize > 0; i++) {

        if (attempt == true && bytes[i] == '1') {
            i++;

            for(int i = 0; i < 10; i++) {
                if (blocks[i] != 0) {
                    bytes[blocks[i]] = '0';
                }

                blocks[i] = 0;
                counter = 0;
                tempSize = fileSize;
                attempt = false;
            }
        }

        else if (bytes[i] == '0') {
            tempSize = tempSize - 512;
            blocks[counter] = i - BLOCK_OFFSET;
            counter++;
            bytes[i] = '1';
            attempt = true;
        }
    }

    if (counter == 0) {
        blocks[0] = 0;
    }

    return blocks;
}

void FileSystem::printTable() {
    for (int i = 0; i < fileTablePosition; i++) {
        cout << bytes[i];
    }
    cout << endl;
}

void FileSystem::writeToSystem(char* buffer, int* blocks) {
    int startingByte = (blocks[0] * 512) - 1;
    for (unsigned int i = 0; i < strlen(buffer); i++) {
        bytes[startingByte] = buffer[i];
        startingByte++;
    }
}

// abstract
void FileSystem::writeToTable(char* targetName, int* blocks) {
    char buffer[3];
    int lastBlock = -1;

    for (unsigned int i = 0; i < strlen(targetName); i++) {
        bytes[fileTablePosition] = targetName[i];
        fileTablePosition++;
    }


    bytes[fileTablePosition] = '|';
    fileTablePosition++;

    for (unsigned int i = 0; i < sizeof(blocks); i++) {
        sprintf(buffer,"%d",blocks[i]);

        if (i == 0) {
            // cout << "first block is " << blocks[i] << endl;
            for (unsigned int j = 0; j < strlen(buffer); j++) {
                    bytes[fileTablePosition] = buffer[j];
                    fileTablePosition++;
            }

            cout << endl;

            bytes[fileTablePosition] = '|';
            fileTablePosition++;
        }

        else if (blocks[i] != 0) {
            lastBlock = i;
        }
    }

    if (lastBlock != -1 && lastBlock != blocks[0]) {
        sprintf(buffer,"%d",blocks[lastBlock]);
    }

    for (unsigned int i = 0; i < strlen(buffer); i++) {
            bytes[fileTablePosition] = buffer[i];
            fileTablePosition++;
    }

    bytes[fileTablePosition] = '\n';
    fileTablePosition++;
}

void FileSystem::printBitmap() {
    for (int i = 512; i < 768 ; i++) {

        cout << bytes[i];

        if (i > 512 && (i + 1) % 32 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

void FileSystem::displayFile(char * fileName) {
    int blockCount;
    int startByte;
    int endByte;
    regex re(fileName);
    smatch m;
    string delimiter = "|";
    int endBlock;
    string fileTable;
    int startBlock;
    string token;

    for (int i = 0; i < 512; i++) {
        fileTable += bytes[i];
    }

    regex_search(fileTable, m, re);

    cout << "match is: " << m[0] << endl;

    // find exact match in file table
    if (m.position(0) != 512 && m[0] == string(fileName)) {
        token = fileTable.substr(m.position(0), fileTable.find('\n'));
        token.erase(0, token.find(delimiter) + delimiter.length());
        startBlock = stoi(token.substr(0, token.find(delimiter)));
        token.erase(0, token.find(delimiter) + delimiter.length());
        endBlock = stoi(token.substr(0, token.find("\n"))); // special delimiter for end of life

        cout << "first block: " << startBlock << endl;
        cout << "last block: " << endBlock << endl;

        blockCount = endBlock - startBlock;

        startByte = 512 * startBlock - 1;
        cout << "start byte: " << startByte << endl;
        endByte = startByte + (512 * (blockCount + 1));
        cout << "last byte: " << endByte << endl;

        for (int i = startByte; i < endByte; i++) {
            cout << bytes[i];
        }

        cout << endl;
    }

    else {

        cout << "File " << fileName << " not found!" << endl;
    }
}
