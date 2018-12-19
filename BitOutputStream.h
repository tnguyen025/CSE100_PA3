#ifndef BITOUTPUTSTREAM_H
#define BITOUTPUTSTREAM_H

#include <iostream>
#include <fstream>
using namespace std;

class BitOutputStream {
private:
  unsigned char buf; // bitwise buffer (8 bits)
  int nbits;         // number of bits that have been written to the buffer
  ostream & out;     // reference to the output stream

public:
  /** constructor
   *  initialize all the fields
   */
  BitOutputStream(ostream & os) : out(os), buf(0), nbits(0) {}

  /** send the bitwise buffer to the output stream, and clear the bitwise
   *  buffer
   */
  void flush();

  /** write bit to the bitwise buffer */
  void writeBit(unsigned int);
};

#endif