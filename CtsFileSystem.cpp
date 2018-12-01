#include "CtsFileSystem.h"

CtsFileSystem::CtsFileSystem() {
    maxBuffer = 512;
    maxSize = 512 * 10;
    for (int i = 0; i < 131072; i++) {

        bytes[i] = '0';
        if (i < 512 && i % FILE_TABLE_FIXED == 0) {
            bytes[i] = '~';
        }
    }
    bytes[512] = '1';
    bytes[513] = '1';
}

void CtsFileSystem::readFile(char* fileName, char* targetName) {
    ifstream is (fileName, ifstream::binary);
    int* blocks;
    int lastByte;

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

            blocks = claimBlocks(length);

            if (blocks[0] == 0) {

                cout << "not enough space" << endl;
            }

            else {

                if (is) {

                    lastByte = writeToSystem(buffer, blocks);
                    writeToTable(targetName, blocks, lastByte);
                    cout << "all characters read successfully.";
                }
                else {

                  cout << endl << "error: only " << is.gcount() << " could be read";
                }
            }

            is.close();

            delete[] buffer;
        }
    }
}

void CtsFileSystem::displayFile(char * fileName) {
    int startByte;
    int* blocks;
    blocks = findFileBlocks(fileName);
    int endByte = blocks[0];
    int startBlock = blocks[1];

    if (startBlock != 0) {

        startByte = 512 * startBlock - 1;

        for (int i = startByte; i < endByte; i++) {

            cout << bytes[i];
        }

        cout << endl;
    }

    else {

        cout << "File " << fileName << " not found!" << endl;
    }
}
