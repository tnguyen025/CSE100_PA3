#include <cmath>
#include "BitInputStream.h"

using namespace std;

const int SIZE_OF_BYTE = 8; // size of a byte (in bits)

// fill the bitwise buffer by reading one byte from the input stream
void BitInputStream::fill() {
  buf = in.get(); // read one byte from istream to bitwise buffer
  nbits = 0;      // no bits have been read from bitwise buffer
}

// read bit from the bitwise buffer
int BitInputStream::readBit() {
  // fill bitwise buffer if there are no more unread bits
  if(nbits == 8) {
      fill();
  }

  // get the next unread bit from the buffer
  int nextBit =
      ((buf & (unsigned char) pow(2, SIZE_OF_BYTE - 1 - nbits)) == 0) ? 0 : 1;
      
  // increment the number of bits that have been read
  nbits++;

  // return the bit we just read
  return nextBit;
}