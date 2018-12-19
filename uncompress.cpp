#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.h"

using namespace std;

int main(int argc, char * const * argv) {
  // create an instance of the ifstream and fstream for input and output from
  // files
  ifstream in;
  ofstream out;
  
  // vector of ints for the frequency
  vector<int> freqs (256, 0);
  
  // to store characters that are written to the out file
  int nextChar;
  int uniqueChar = 0; // number of unique characters
  int lastUniqueChar = 0; // keep track of the unique symbol in case theres
                          // only 1
  unsigned int outputFileSize = 0; // size of the file
  
  // check the number of command line arguments, print error prompt and exit
  // if not 3 (including the file name)
  if (argc != 3) {
    cout << argv[0] << " called with incorrect arguments.\n"
         << "Usage: " << argv[0] << " infile outfile\n";
         
    return 1;
  }
  
  // open input file to read
  in.open(argv[1], ios::binary);
  in.seekg(0, ios::beg);
  
  // if input file does not exist, output error and exit
  if (!in.is_open()) {
    cout << "Error opening \"0\".\n";
    
    return 1;
  }
   
  // read the header
  unsigned int i;
  for (i = 0; i < freqs.size(); i++) {
    unsigned char bytesToRead = in.get(); // get number of bytes to read
  
    if (!in.good())
      break;
      
    // if bytesToRead is not 0
    if (bytesToRead) {
      in.read((char *) & freqs[i], bytesToRead);
      uniqueChar++; // one more unique symbol
      lastUniqueChar = i;
      outputFileSize += freqs[i]; // increased output file's size
    }
  }
  
  // if header if not correct then exit
  if (i < freqs.size()) {
    cout << "Reading header from file \"" << argv[1] << "\""
         << "\"...  Problem: EOF while reading header.\n";
    return 1;
  }  

  cout << "Reading from file \"" << argv[1] << "\"... done.\n"
       << "Uncompress file will have " << uniqueChar
       << " unique symbols and size "
       << outputFileSize << " bytes.\n"; 
  
  // create an instance of the HCTree and build the tree
  HCTree myTree;
  myTree.build(freqs);
  
  cout << "Building Huffman code tree... done.\n";
  
  // create an instance of BitInputStream
  BitInputStream inBit(in);
   
  // open output file to write
  out.open(argv[2], ios::binary);
  out.seekp(0, ios::beg);
  
  if (outputFileSize) {
    for (unsigned int i = 0; i < outputFileSize; i++) {
      // printing the only symbol in this edge case (1 unique symbol)
      if (uniqueChar == 1) {
        out.put(lastUniqueChar);
      }
      
      else {
        nextChar = myTree.decode(inBit);
        
        out.put(nextChar);
      }
    }
  }

  // get input file size
  in.clear();
  in.seekg(0, ios::beg);
  unsigned int inputFileStart = in.tellg();
  in.seekg(0, ios::end);
  unsigned int inputFileEnd = in.tellg();
  unsigned int inputFileSize = inputFileEnd - inputFileStart;
  
   cout << "Writing to file \"" << argv[2] << "\"... done.\n"
        << "Uncompression ratio: " << setprecision(6)
        << outputFileSize / (double) inputFileSize
        << endl;
        
   in.close();
   out.close();
}
