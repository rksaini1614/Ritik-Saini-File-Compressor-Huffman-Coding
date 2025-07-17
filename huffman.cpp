#include "huffman.hpp"
#include<vector>
using namespace std;


// function to create vector of tree
void Huffman::createVector() {
    for(int i=0;i<128;i++){
        Node* n = new Node();
        n->data = i;
        n->freq = 0;
        arr.push_back(n);
    }
}

// function to traverse the huffman tree
void Huffman::traverse(Node* root,string str) {
    if(root->left == NULL && root->right == NULL){
        root->code = str;
        return;
    }

    traverse(root->left,str + '0');
    traverse(root->right,str + '1');
}

// function to convert binary string to decimal number
int Huffman::binaryToDecimal(string binary){
    int res = 0;
    for(auto ch : binary) {
        res = res*2 + (ch-'0');
    }
    return res;
}

// function to convert decimal number to binary string
// string Huffman::decimalToBinary(int num) {
//     string temp = "";
//     string res = "";
//     while(num > 0) {
//         temp += (num % 2 + '0');
//         num /= 2;
//     }

//     res.append(8 - temp.length(), '0');
//     for(int i=temp.length()-1;i>=0;i--) {
//         res += temp[i];
//     }

//     return res;
// }
string Huffman::decimalToBinary(int num) {
    string res = "";
    for (int i = 7; i >= 0; i--) {
        res += ((num >> i) & 1) ? '1' : '0';
    }
    return res;
}


// function to build hoofman tree
void Huffman::buildTree(char a_code, string& path) {
    Node* curr = root;
    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '0') {
            if (curr->left == NULL) {
                curr->left = new Node();
            }
            curr = curr->left;
        }
        else if (path[i] == '1') {
            if (curr->right == NULL) {
                curr->right = new Node();
            }
            curr = curr->right;
        }
    }
    curr->data = a_code;
}

// create min heap for huffman tree
void Huffman::createMinHeap() {
    char id;
    inputFile.open(inputFileName, ios::in);
    inputFile.get(id);
    //Incrementing frequency of characters that appear in the input file
    while (!inputFile.eof()) {
        arr[id]->freq++;
        inputFile.get(id);
    }
    inputFile.close();
    //Pushing the Nodes which appear in the file into the priority queue (Min Heap)
    for (int i = 0; i < 128; i++) {
        if (arr[i]->freq > 0) {
            minHeap.push(arr[i]);
        }
    }
}

// function to create huffman tree
void Huffman::createTree() {
    //Creating Huffman Tree with the Min Heap created earlier
    Node *left, *right;
    priority_queue <Node*, vector<Node*>, Compare> tempPQ(minHeap);
    while (tempPQ.size() != 1)
    {
        left = tempPQ.top();
        tempPQ.pop();
            
        right = tempPQ.top();
        tempPQ.pop();
            
        root = new Node();
        root->freq = left->freq + right->freq;

        root->left = left;
        root->right = right;
        tempPQ.push(root);
    }
}

// function to generate code for a characters in file
void Huffman::createCodes() {
    traverse(root, "");
}

// function to save encoded file
void Huffman::saveEncodedFile() {
    //Saving encoded (.huf) file
    inputFile.open(inputFileName, ios::in);
    outputFile.open(outputFileName, ios::out | ios::binary);
    string in = "";
    string s = "";
    char id;

    //Saving the meta data (huffman tree)
    in += (char)minHeap.size();
    priority_queue <Node*, vector<Node*>, Compare> tempPQ(minHeap);
    while (!tempPQ.empty()) {
        Node* curr = tempPQ.top();
        in += curr->data;

        //Saving 16 decimal values representing code of curr->data
        s.assign(127 - curr->code.length(), '0');
        s += '1';
        s += curr->code;
        
        //Saving decimal values of every 8-bit binary code 
        in += (char)binaryToDecimal(s.substr(0, 8));
        for (int i = 0; i < 15; i++) {
            s = s.substr(8);
            in += (char)binaryToDecimal(s.substr(0, 8));
        }
        tempPQ.pop();
    }
    s.clear();

    //Saving codes of every charachter appearing in the input file
    inputFile.get(id);
    while (!inputFile.eof()) {
        s += arr[id]->code;
        //Saving decimal values of every 8-bit binary code
        while (s.length() > 8) {
            in += (char)binaryToDecimal(s.substr(0, 8));
            s = s.substr(8);
        }
        inputFile.get(id);
    }

    //Finally if bits remaining are less than 8, append 0's
    int count = 8 - s.length();
	if (s.length() < 8) {
		s.append(count, '0');
	}
	in += (char)binaryToDecimal(s);	
    //append count of appended 0's
    in += (char)count;

    //write the in string to the output file
	outputFile.write(in.c_str(), in.size());
	inputFile.close();
	outputFile.close();
}

void Huffman::saveDecodedFile() {
    inputFile.open(inputFileName, ios::in | ios::binary);
    outputFile.open(outputFileName, ios::out);
    unsigned char size;
    inputFile.read(reinterpret_cast<char*>(&size), 1);
    //Reading count at the end of the file which is number of bits appended to make final value 8-bit
    inputFile.seekg(-1, ios::end);
    char count0;
    inputFile.read(&count0, 1);
    //Ignoring the meta data (huffman tree) (1 + 17 * size) and reading remaining file
    inputFile.seekg(1 + 17 * size, ios::beg);

    vector<unsigned char> text;
    unsigned char textseg;
    inputFile.read(reinterpret_cast<char*>(&textseg), 1);
    while (!inputFile.eof()) {
        text.push_back(textseg);
        inputFile.read(reinterpret_cast<char*>(&textseg), 1);
    }

    Node *curr = root;
    string path;
    for (int i = 0; i < text.size() - 1; i++) {
        //Converting decimal number to its equivalent 8-bit binary code
        path = decimalToBinary(text[i]);
        if (i == text.size() - 2) {
            path = path.substr(0, 8 - count0);
        }
        //Traversing huffman tree and appending resultant data to the file
        for (int j = 0; j < path.size(); j++) {
            if (path[j] == '0') {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }

            if (curr->left == NULL && curr->right == NULL) {
                outputFile.put(curr->data);
                curr = root;
            }
        }
    }
    inputFile.close();
    outputFile.close();
}

void Huffman::getTree() {
    inputFile.open(inputFileName, ios::in | ios::binary);
    //Reading size of MinHeap
    unsigned char size;
    inputFile.read(reinterpret_cast<char*>(&size), 1);
    root = new Node();
    //next size * (1 + 16) characters contain (char)data and (string)code[in decimal]
    for(int i = 0; i < size; i++) {
        char aCode;
        unsigned char hCodeC[16];
        inputFile.read(&aCode, 1);
        inputFile.read(reinterpret_cast<char*>(hCodeC), 16);
        //converting decimal characters into their binary equivalent to obtain code
        string hCodeStr = "";
        for (int i = 0; i < 16; i++) {
            hCodeStr += decimalToBinary(hCodeC[i]);
        }
        //Removing padding by ignoring first (127 - curr->code.length()) '0's and next '1' character
        int j = 0;
        while (hCodeStr[j] == '0') {
            j++;
        }
        hCodeStr = hCodeStr.substr(j+1);
        //Adding node with aCode data and hCodeStr string to the huffman tree
        buildTree(aCode, hCodeStr);
    }
    
    inputFile.close();
}

void Huffman::compress() {
    createMinHeap();
    createTree();
    createCodes();
    saveEncodedFile();
}

void Huffman::decompress() {
    getTree();
    saveDecodedFile();
}