#include "rbt.h"

// Set implemented with Red-Black tree
// functionality:
//		Insert and delete elements by value
//		Find whether element is present in set
//		Read it's size
//		Clear content
// More detailed information about functions is availablie in bst.h header

// default RBT set
template <typename T> class RBTSet : public RBTBaseSet<T, RBTNode<T>> {};

// default set
template <typename T> class Set : public RBTSet<T> {};