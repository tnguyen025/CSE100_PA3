#include <cmath>
#include "BitOutputStream.h"

using namespace std;

const int SIZE_OF_BYTE = 8; // size of a byte (in bits)

/** send the bitwise buffer to the output stream, and clear the bitwise
 *  buffer
 */
void BitOutputStream::flush() {
  if (nbits) {    
    out.put(buf);  // write the buffer to the ofstream
    out.flush();   // flush the stream
    buf = 0;       // clear the bitwise buffer
    nbits = 0;     // bitwise buffer is cleared, so there are 0 bits in it
  }
}

/** write bit to the bitwise buffer */
void BitOutputStream::writeBit(unsigned int bit) {  
  // flush the bitwise buffer if it is full
  if(nbits == 8) {
    flush();
  }

  // set the next open bit of the bitwise buffer to 'bit'
  if (bit)
    buf = buf | (unsigned char) pow(2, SIZE_OF_BYTE - 1 - nbits);
  
  nbits++;
}