#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <string>
#include <vector>
#include <queue>
#include <fstream>

// Defining the Huffman Tree Node structure
struct Node {
    char data;
    unsigned freq;
    std::string code;
    Node* left;
    Node* right;

    Node() {
        left = right = nullptr;
    }
};

// Defining the Huffman Class
class Huffman {
private:
    std::vector<Node*> arr;
    std::fstream inputFile, outputFile;
    std::string inputFileName, outputFileName;
    Node* root;

    class Compare {
    public:
        bool operator()(Node* left, Node* right) {
            return left->freq > right->freq;
        }
    };

    std::priority_queue<Node*, std::vector<Node*>, Compare> minHeap;

    // function to initialize vector of tree nodes representing character's ascii value and freq as 0
    void createVector();  

    // function to traverse Huffman tree
    void traverse(Node*, std::string);

    // function to convert binary string to equivalent decimal number
    int binaryToDecimal(std::string);

    // function to convert decimal number to equivalent binary string
    std::string decimalToBinary(int);

    // Reconstruct the Huffman tree while decoding the file
    void buildTree(char, std::string&);

    // function to create a minheap of nodes according to the frequency of chars in input file
    void createMinHeap();

    // Function to create Huffman tree
    void createTree();

    // function to generate huffman codes
    void createCodes();

    // function to save the encoded file
    void saveEncodedFile();

    // function to save the decoded file
    void saveDecodedFile();

    // function to read the file to recontrust the huffman tree
    void getTree();

public:
    Huffman(std::string inputFileName, std::string outputFileName) {
        this->inputFileName = inputFileName;
        this->outputFileName = outputFileName;
        createVector();
    }

    // function to compress the input file
    void compress(); 

    // function to decompress the input file
    void decompress();  
};

#endif
