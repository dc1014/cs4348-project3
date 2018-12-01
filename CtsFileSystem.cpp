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
