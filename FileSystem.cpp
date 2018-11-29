#include "FileSystem.h"

FileSystem::FileSystem() {
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

// abstract
void FileSystem::readFile(char* fileName, char* targetName) {
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

void FileSystem::writeFile(char* fileName, char* targetName) {
    int* blocks;
    int startByte;
    ofstream os (targetName);
    blocks = findFileBlocks(fileName);
    int startBlock = blocks[1];
    int endByte = blocks[0];

    if (os && blocks[0] != 0) {

        startByte = 512 * startBlock - 1;

        for (int i = startByte; i < endByte; i++) {
            os << bytes[i];
        }

        cout << endl;
        os.close();
    }

    else {

        cout << "File not found!" << endl;
    }
}

// abstract
int* FileSystem::claimBlocks(int fileSize) {
    int * blocks = new int[10];

    for (int i = 0; i < 10; i++) {
        blocks[i] = 0;
    }

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
    for (int i = 0; i < 512; i++) {

        cout << bytes[i];
    }
    cout << endl;
}

int FileSystem::writeToSystem(char* buffer, int* blocks) {
    int startingByte = ((blocks[0] - 1) * 512 );
    for (unsigned int i = 0; i < strlen(buffer); i++) {

        bytes[startingByte] = buffer[i];
        startingByte++;
    }
    return startingByte;
}

// abstract
void FileSystem::writeToTable(char* targetName, int* blocks, int lastByte) {
    char buffer[20];
    int lastBlock = -1;
    int fileTablePosition = -1;
    int counter = 0;
    while (fileTablePosition == -1 && counter < 512) {
        if (bytes[counter] == '~') {
            fileTablePosition = counter;
        }
        else {
            counter++;
        }
    }

    for (unsigned int i = 0; i < strlen(targetName); i++) {
        bytes[fileTablePosition] = targetName[i];
        fileTablePosition++;
    }

    bytes[fileTablePosition] = '|';
    fileTablePosition++;

    sprintf(buffer,"%d",lastByte);

    for (unsigned int i = 0; i < strlen(buffer); i++) {
        bytes[fileTablePosition] = buffer[i];
        fileTablePosition++;
    }

    bytes[fileTablePosition] = '|';
    fileTablePosition++;

    for (unsigned int i = 0; i < sizeof(blocks); i++) {

        sprintf(buffer,"%d",blocks[i]);

        if (i == 0) {

            for (unsigned int j = 0; j < strlen(buffer); j++) {

                    bytes[fileTablePosition] = buffer[j];
                    fileTablePosition++;
            }

            cout << endl;

            bytes[fileTablePosition] = '|';
            fileTablePosition++;
            lastBlock = i;
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

int * FileSystem::findFileBlocks(char * fileName) {
    int* blocks = new int[11];
    smatch m;
    string delimiter = "|";
    int endBlock;
    string fileTable;
    int startBlock;
    int endByte;
    string token;
    string tempName = fileName;
    tempName += "\\|";

    regex re(tempName);

    for (int i = 0; i < 512; i++) {

        fileTable += bytes[i];
    }

    regex_search(fileTable, m, re);

    // find exact match in file table
    if (m.position(0) != 512) {

        blocks[3] = m.position(0); // starting position of liine on File table
        token = fileTable.substr(m.position(0), fileTable.find('\n'));
        blocks[4] = token.length(); // ending position of line on file table
        token.erase(0, token.find(delimiter) + delimiter.length());
        endByte = stoi(token.substr(0, token.find(delimiter)));
        token.erase(0, token.find(delimiter) + delimiter.length());
        startBlock = stoi(token.substr(0, token.find(delimiter)));
        token.erase(0, token.find(delimiter) + delimiter.length());
        endBlock = stoi(token.substr(0, token.find("\n"))); // special delimiter for end of life
        blocks[0] = endByte;
        blocks[1] = startBlock;
        blocks[2] = endBlock;
    }

    return blocks;
}

void FileSystem::displayFile(char * fileName) {
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

void FileSystem::printBlock(int block) {
    int startByte = 512 * (block - 1);

    for (int i = startByte; i < startByte + 512; i++) {
        cout << bytes[i];
    }

    cout << endl;
}

void FileSystem::deleteFile(char* fileName) {
    int startByte;
    int* blocks;
    blocks = findFileBlocks(fileName);
    int endByte = blocks[0];
    int startBlock = blocks[1];
    int endBlock = blocks[2];

    // find exact match in file table
    if (blocks[0] != 0) {

        startByte = 512 * (startBlock - 1);

        for (int i = startByte; i < endByte; i++) {

            bytes[i] = '0';
        }

        for (int i = blocks[3]; i < blocks[3] + FILE_TABLE_FIXED; i++) {
            bytes[i] = '0';
            if (i == blocks[3]) {
                bytes[i] = '~';
            }
        }

        for (int i = startBlock - 1; i <= endBlock; i++) {

            bytes[i + 512] = '0';
        }
    }

    else {
        cout << "File " << fileName << " not found!" << endl;
    }
}
