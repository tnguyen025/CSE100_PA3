#ifndef BITINPUTSTREAM_H
#define BITINPUTSTREAM_H

#include <iostream>
#include <fstream>
using namespace std;

class BitInputStream {
private:
  unsigned char buf;  // bitwise buffer (8 bits)
  int nbits;          // number of bits that have been read
  istream & in;       // reference to the input stream

public:
  // constructor: assign 'in' to 'is', 'buf' to 0, and 'nbits' to 8
  BitInputStream(istream & is) : in(is), buf(0), nbits(8) {}

  // fill the buffer by reading a byte from the input stream
  void fill();

  // read bit from the bitwise buffer
  int readBit();
};

#endif