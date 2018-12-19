#include "HCTree.h"

using namespace std;

/** Destructor
 *  delete all the tree
 */
HCTree::~HCTree() {
  if (root)
    root->deleteAll();
}

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {
  // the forest of the nodes and the nodes pointers to combine nodes together
  priority_queue<HCNode *, vector<HCNode *>, HCNodePtrComp> forest;
  HCNode * node;
  HCNode * left;
  HCNode * right;
  
  for (unsigned int i = 0; i < freqs.size(); i++) {
    if (freqs[i]) {
      leaves[i] = new HCNode(freqs[i], i);
      forest.push(leaves[i]);
    }
  }
  
  // exits if there is no unique symbol at all
  if (forest.empty())
    return;
  
  // if there is only 1 unique symbol
  if (forest.size() == 1) {
    // get the symbol and assign it to right
    right = forest.top();
    forest.pop();
    
    // create a new root and make the symbol node the root's right child
    root = new HCNode(right->count, right->symbol);
    root->c1 = right;
    right->p = root;
    
    return;
  }
  
  // otherwise combine 2 lowest count symbols until there is only 1 left
  while (forest.size() > 1) {
    // get the 2 symbols with smallest count
    left = forest.top();
    forest.pop();
    right = forest.top();
    forest.pop();
    
    // combine them to a new node and push back to the heap
    node = new HCNode(left->count + right->count, left->symbol);
    node->c0 = left;
    node->c1 = right;
    left->p = node;
    right->p = node;
    forest.push(node);
  }
  
  // when there is only 1 node left in the forest then it is the root
  root = forest.top();
  forest.pop();
  
}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
  if (leaves[symbol]->p)
    leaves[symbol]->getCode(leaves[symbol]->p, out);
}

/** Write to the given ofstream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
 *  BE USED IN THE FINAL SUBMISSION.
 */
void HCTree::encode(byte symbol, ofstream& out) const {
  if (leaves[symbol]->p)
    leaves[symbol]->getCode(leaves[symbol]->p, out);
}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in) const {
  int nextBit = -1;
  
  // create a pointer that originally points to root
  HCNode * curr = root;
  
  // read the input sequence until we find a leaf node
  while (curr->c0 || curr->c1) {
    nextBit = in.readBit();

    if (nextBit == 0)
      curr = curr->c0;
    else if (nextBit == 1)
      curr = curr->c1;
  }
  
  // now curr is a leaf node, store curr's symbol in nextChar and return
  return curr->symbol;
}

/** Return the symbol coded in the next sequence of bits (represented as 
 *  ASCII text) from the ifstream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
 *  IN THE FINAL SUBMISSION.
 */
int HCTree::decode(ifstream& in) const {
  int nextChar;
  
  // create a pointer that originally points to root
  HCNode * curr = root;
  
  // read the input sequence until we find a leaf node
  while (curr->c0 || curr->c1) {
    nextChar = in.get();
    
    if (in.eof())
      return 0;
    
    if (nextChar == '0')
      curr = curr->c0;
    else if (nextChar == '1')
      curr = curr->c1;
  }
  
  // now curr is a leaf node, store curr's symbol in nextChar and return
  nextChar = curr->symbol;
  
  return nextChar;
}