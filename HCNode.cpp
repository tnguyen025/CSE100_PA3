#include "HCNode.h"

using namespace std;


/** Less-than comparison, so HCNodes will work in std::priority_queue
 *  We want small counts to have high priority.
 *  And we want to break ties deterministically.
 */
bool HCNode::operator<(const HCNode& other) {
  if (this->count > other.count)
    return true;
  
  else if (this->count == other.count)
    if (this->symbol > other.symbol)
      return true;
    
  return false;
}

/** Delete all the nodes */
void HCNode::deleteAll() {
  // delete its 0 child
  if (this->c0)
    this->c0->deleteAll();
  // delete its 1 child
  if (this->c1)
    this->c1->deleteAll();
  
  // delete the node it self
  delete this;
}