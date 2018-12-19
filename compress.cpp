#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "HCTree.h"

using namespace std;

const int MAX_ONE_BYTE = 255;
const int MAX_TWO_BYTE = 65535;
const int MAX_THREE_BYTE = 16777215;
const int MAX_FOUR_BYTE = 4294967295;

int main(int argc, char * * argv) {
  // create an instance of the ifstream and fstream for input and output from
  // files
  ifstream in;
  ofstream out;
  
  // vector of ints for the frequency
  vector<int> freqs (256, 0);
  
  // to store bytes that are read from the input file
  unsigned char nextByte;
  int uniqueByte = 0; // number of unique characters
  int inputFileSize = 0; // size of the file
  
  // check the number of command line arguments, print error prompt and exit
  // if not 3 (including the file name)
  if (argc != 3) {
    cout << argv[0] << " called with incorrect arguments.\n"
         << "Usage: " << argv[0] << " infile outfile\n";
         
    return 1;
  }
  
  // open input file to read
  in.open(argv[1], ios::binary);
  
  // if input file does not exist, output error and exit
  if (!in.is_open()) {
    cout << "Error opening \"0\".\n";
    
    return 1;
  }
  
  // set the cursor to the beginning of the file
  in.seekg(0, ios::beg);
  
  // read the file
  nextByte = in.get();
  while (!in.eof()) {
    // if the byte is not found before then it is unique
    if (freqs[nextByte] == 0) {
      uniqueByte++;
    }
    
    // increase the frequency of the byte
    freqs[nextByte]++;
    
    nextByte = in.get(); // read the next byte
    
    inputFileSize++;
  }
  
  cout << "Reading from file \"" << argv[1] << "\"... done.\n"
       << "Found " << uniqueByte << " unique symbols in input file of size "
       << inputFileSize << " bytes.\n";
 
  // create an instance of the HCTree and build the tree
  HCTree myTree;
  myTree.build(freqs);
  
  cout << "Building Huffman code tree... done.\n";
  
  // open output file to write
  out.open(argv[2], ios::binary);
  out.seekp(0, ios::beg);
  
  // construct the header
  for (int i = 0; i < 256; i++) {
    // if the byte i was not in the input file
    if (!freqs[i]) {
      out.put(0);
    }
  
    else if (freqs[i] <= MAX_ONE_BYTE) {
      out.put(1); //tell the reader to only read the next 1 byte
      out.write((char *) &freqs[i], 1);
    }
    
    else if (freqs[i] <= MAX_TWO_BYTE) {
      out.put(2); //tell the reader to only read the next 2 bytes
      out.write((char *) &freqs[i], 2);
    }
    
    else if (freqs[i] <= MAX_THREE_BYTE) {
      out.put(3); //tell the reader to only read the next 3 bytes
      out.write((char *) &freqs[i], 3);
    }
    
    else if (freqs[i] <= MAX_FOUR_BYTE) {
      out.put(4); //tell the reader to only read the next 4 bytes
      out.write((char *) &freqs[i], 4);
    }
  }
 
  // create an instance of BitOutput Stream
  BitOutputStream outBit(out);
  
  // if there are more than just 1 unique symbol then do the encoding
  if (uniqueByte > 1) {
    // read the message from input file and write the encoded message to the
    // output file
    in.clear();
    in.seekg(0, ios::beg);
    nextByte = in.get();
    while (!in.eof()) {
      myTree.encode((byte) nextByte, outBit);
      
      nextByte = in.get(); // read the next character
    }
    
    // write whatever left in the bitwise stream buffer to the output file
    outBit.flush();
  }  
  
  cout << "Writing to file \"" << argv[2] << "\"... done.\n"
       << "Output file has size " << out.tellp() << " bytes.\n"
       << "Compression ratio: ";
       
  if (inputFileSize == 0)
    cout << "inf\n";
  
  else
    cout << out.tellp() / (double) inputFileSize << endl;
}