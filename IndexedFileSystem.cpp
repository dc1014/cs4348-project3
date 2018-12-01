#include "IndexedFileSystem.h"

IndexedFileSystem::IndexedFileSystem() {
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
void IndexedFileSystem::readFile(char* fileName, char* targetName) {
    cout << "poly!" << endl;
    ifstream is (fileName, ifstream::binary);
    vector<int> blocks;
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

// TO DO: prevent over allocation
vector<int> IndexedFileSystem::claimBlocks(int fileSize) {
    vector<int> blocks;
    vector<int> claimedBlocks;
    int tempSize = fileSize;
    int counter = 0;
    srand (time(NULL));
    int randomNumber;
    int currentBlock;

    for(int i = 514; i < 768; i++) {
        if (bytes[i] == '0') blocks.push_back(i);
    }

    while (counter < 10 && tempSize > 0) {
        randomNumber = rand() % blocks.size() + 1;
        currentBlock = blocks.at(randomNumber) - BLOCK_OFFSET;
        claimedBlocks.push_back(currentBlock);
        bytes[currentBlock + BLOCK_OFFSET] = '1';
        blocks.erase(blocks.begin() + randomNumber, blocks.begin() + randomNumber);
        counter++;
        tempSize = tempSize - 512;

    }

    return claimedBlocks;
}

void IndexedFileSystem::writeFile(char* fileName, char* targetName) {
    vector<int> blocks;
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

int IndexedFileSystem::writeToSystem(char* buffer, vector<int> blocks) {
    int currentByte;
    unsigned int bufferPosition = 0;

    for (auto it = blocks.begin(); it != blocks.end(); it++) {
            currentByte = blockStart(*it);
            bytes[currentByte] = buffer[bufferPosition];
            currentByte++;
            bufferPosition++;
            cout << "current block " << *it << endl;

            while(currentByte % 512 != 0 && bufferPosition < strlen(buffer)) {
                bytes[currentByte] = buffer[bufferPosition];
                currentByte++;
                bufferPosition++;
            }
    }

    return currentByte;
}

// abstract
void IndexedFileSystem::writeToTable(char* targetName, vector<int> blocks, int lastByte) {
    char buffer[20];
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

    for (auto it = blocks.begin(); it != blocks.end(); it++) {
        sprintf(buffer,"%d",*it);
        for (unsigned int i = 0; i < strlen(buffer); i++) {

                bytes[fileTablePosition] = buffer[i];
                fileTablePosition++;
        }
        if ((it != blocks.end()) && (next(it) != blocks.end())) {
            bytes[fileTablePosition] = '|';
            fileTablePosition++;
        }
    }

    bytes[fileTablePosition] = '\n';
    fileTablePosition++;
}

vector<int> IndexedFileSystem::findFileBlocks(char * fileName) {
    vector<int> blocks;
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

void IndexedFileSystem::displayFile(char * fileName) {
    int startByte;
    vector<int> blocks;
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

void IndexedFileSystem::deleteFile(char* fileName) {
    int startByte;
    vector<int> blocks;
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
