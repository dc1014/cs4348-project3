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

            cout << endl << "Reading " << length << " characters... ";

            // read data as a block:
            is.read (buffer,length);

            cout << buffer << endl;

            blocks = findBlocks(length);

            if (blocks[0] == '0') {
                cout << "not enough space" << endl;
            }

            else {

                if (is) {
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

    for(int i = 514; i < 768 && counter < 10 && fileSize > 0; i++) {

        if (bytes[i] == '0') {
            fileSize = fileSize - 512;
            blocks[counter] = i - BLOCK_OFFSET;
            counter++;
            bytes[i] = '1';
        }
    }

    if (counter == 0) {
        blocks[0] = '0';
    }

    cout << blocks[0] << endl;

    return blocks;
}

void FileSystem::printTable() {
    for (int i = 0; i < fileTablePosition; i++) {
        cout << bytes[i];
    }
    cout << endl;
}

// abstract
void FileSystem::writeToTable(char* targetName, int* blocks) {
    for (unsigned int i = 0; i < sizeof(targetName); i++) {
        bytes[fileTablePosition] = targetName[i];
        fileTablePosition++;
    }

    bytes[fileTablePosition] = '\t';
    fileTablePosition++;

    bytes[fileTablePosition] =
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
