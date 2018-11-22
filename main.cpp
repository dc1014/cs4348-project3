#include <fstream>
#include <iostream>
#include "FileSystem.h"

using namespace std;

int main(int argc, char* argv[]) {
    int input;

    FileSystem fs;
    char* inFileName = new char[8];
    char* copyFileName = new char[8];

    // ifstream infile("./test.txt", ios::in | ios::binary);
    // infile.seekg(0, infile.end);
    // size_t length = infile.tellg();
    // infile.seekg(0, infile.beg);

    // cout << length << endl;

    // if (length > sizeof (buffer)) {
    //     length = sizeof (buffer);
    // }

    // infile.read(buffer,length);
    // infile.close();
    // cout << buffer;

    while (input != 8) {
        cout << "1) Display a file" << endl;
        cout << "2) Display the file table" << endl;
        cout << "3) Display the free space bitmap" << endl;
        cout << "4) Display a disk block" << endl;
        cout << "5) Copy a file from the simulation to a file on the real system" << endl;
        cout << "6) Copy a file from the real system to a file in the simulation" << endl;
        cout << "7) Delete a file" << endl;
        cout << "8) Exit" << endl;

        cin >> input;

        cout << "\n Choice " << input << endl;

        switch(input) {
            case 2:
                fs.printTable();
                break;
            case 3:
                fs.printBitmap();
                break;
            case 6:
                cout << "Copy from: ";
                cin >> inFileName;
                cout << "Copy to: ";
                cin >> copyFileName;
                fs.readFile(inFileName, copyFileName);
                cout << endl << "File " << inFileName << " copied" << endl;
                break;
            case 8:
                cout << "terminating" << endl;
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }

    }

    return 0;

}
