#include <iostream>
#include "huffman.hpp"
using namespace std;



int main() {
    int choice;
    string inputPath, outputPath;

    while (true) {
        cout << "\n--------------- File Compressor using Huffman Coding ---------------" << endl;
        cout << "1. Compress File" << endl;
        cout << "2. Decompress File" << endl;
        cout << "3. Exit" << endl;
        cout<< "-------------------------------------------------------------------------" << endl;
        cout << "Enter your choice : ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "Enter path of the input file: ";
            getline(cin, inputPath);

            cout << "Enter path to save the compressed file: ";
            getline(cin, outputPath);

            Huffman f(inputPath, outputPath);
            f.compress();
            cout << "Your file is compressed successfully..!" << endl;

        } 
        else if (choice == 2) {
            cout << "Enter path of the compressed (.huf) file: ";
            getline(cin, inputPath);

            cout << "Enter path to save the decompressed file: ";
            getline(cin, outputPath);

            Huffman f(inputPath, outputPath);
            f.decompress();
            cout << "Your file is decompressed successfully..!" << endl;

        }
        else if (choice == 3) {
            cout << "Exiting..! Thank for using File Compressor." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please enter a valid choice." << endl;
        }
    }

    return 0;
}
