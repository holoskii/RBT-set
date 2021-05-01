/* Basic functionality of binary search tree */

#ifndef BST_HEADER_
#define BST_HEADER_

#include <iostream>
#include <iomanip>
#include <cassert>

template <typename T, typename N>
class BST {
public:
	// Constructor, creates empty tree
	BST();

	// Constructor, creates tree with one value
	BST(const T& value);

	// Destructor
	~BST();

	// Returns number of elements in tree
	size_t size();

	// Returns true if tree is empty, false otherwise
	bool empty();

	// Return number of elements having the same value
	size_t count(const T& value);

	// Inserts new elements with given value
	N* insert(const T& value);

	// Erases element with given value
	virtual void erase(const T& value);
	
	// Gets value of smallest element
	T getMin();
	
	// Gets value of biggest element
	T getMax();

	// Print tree in human readable form
	void print();

	// Clear tree content
	void clear();

protected:
	// Current number of elements in tree
	size_t numElements = 0;

	// Pointer at node which is tree's root
	N* root = nullptr;
	
	// Insert node at current node
	// Return pointer of inserted node
	virtual N* insertNode(N* node, N* newNode);

	// Find node with given value among descendants of given node
	// Return pointer of found node or nullptr otherwise
	N* find(N* node, const T& value);

	// Delete node by its pointer
	virtual N* deleteNode(N* node);

	// Get node with smallest value among descendants of given node
	N* getMinNode(N* node);

	// Get node with biggest value among descendants of given node
	N* getMaxNode(N* node);

	// Recursively prints tree
	void printTree(const std::string& prefix, N* node, bool isLeft);
	
	// Do left rotation of current node
	N* leftRotate(N* node);

	// Do right rotation of current node
	N* rightRotate(N* node);

	// Free memory occupied by node and it's ancestors
	void freeNode(N* node);
};

template <typename T, typename N>
BST<T, N>::BST() {}

template <typename T, typename N>
BST<T, N>::BST(const T& value) {
	root = new N<T>(value);
}

template <typename T, typename N>
BST<T, N>::~BST() {
	freeNode(root);
}

template <typename T, typename N>
void BST<T, N>::clear() {
	freeNode(root);
	root = nullptr;
	numElements = 0;
}

template <typename T, typename N>
size_t BST<T, N>::size() {
	return numElements;
}

template <typename T, typename N>
bool BST<T, N>::empty() {
	return (numElements == 0);
}

template <typename T, typename N>
size_t BST<T, N>::count(const T& value) {
	return find(root, value) != nullptr;
}

template <typename T, typename N>
N* BST<T, N>::insert(const T& value) {
	N* newNode = find(value);
	if (newNode) {
		return newNode;
	}
	numElements++;
	newNode = new N(value);
	root = insertNode(root, newNode);
	return newNode;
}

template <typename T, typename N>
void BST<T, N>::erase(const T& value) {
	N* node = BST::find(BST::root, value);
	if (!node) {
		return;
	}
	numElements--;
	deleteNode(node);
}

template <typename T, typename N>
void BST<T, N>::print() {
	printTree("", this->root, false);
	std::cout << std::endl;
}

template <typename T, typename N>
T BST<T, N>::getMin() {
	return getMinNode(root)->value;
}

template <typename T, typename N>
T BST<T, N>::getMax() {
	return getMaxNode(root)->value;
}

template <typename T, typename N>
N* BST<T, N>::insertNode(N* node, N* newNode) {
	if (!node) {
		return newNode;
	}
	if (newNode->value < node->value) {
		node->left = insertNode(static_cast<N*>(node->left), newNode);
		node->left->parent = node;
	}
	else {
		node->right = insertNode(static_cast<N*>(node->right), newNode);
		node->right->parent = node;
	}
	return node;
}

template <typename T, typename N>
N* BST<T, N>::find(N* node, const T& value) {
	if (!node) {
		return nullptr;
	}
	if (node->value == value) {
		return node;
	}
	if (value < node->value) {
		return find(node->left, value);
	}
	return find(node->right, value);
}

template <typename T, typename N>
N* BST<T, N>::deleteNode(N* node) {
	N* parentNode = nullptr;
	// no children, remove node and balance parent
	if (!node->left && !node->right) {
		if (BST::root == node) {
			BST::root = nullptr;
		}
		else {
			assert(node->parent);
			if (node->parent->left == node) {
				node->parent->left = nullptr;
			}
			else {
				node->parent->right = nullptr;
			}
		}
		parentNode = node->parent;
		free(node);
	}
	// both children, balance itself
	else if (node->left && node->right) {
		N* successor = BST::getMinNode(node->right);
		int value = successor->value;
		deleteNode(successor);
		node->value = value;
		parentNode = node;
	}
	// one child, balance parent
	else {
		N* child = node->left ? node->left : node->right;
		if (BST::root == node) {
			BST::root = child;
			child->parent = nullptr;
		}
		else {
			if (node->parent->left == node) {
				node->parent->left = child;
			}
			else {
				node->parent->right = child;
			}
			child->parent = node->parent;
		}
		parentNode = node->parent;
		free(node);
	}
	return parentNode;
}

template <typename T, typename N>
N* BST<T, N>::getMinNode(N* node) {
	N* result = node;
	if (!result) {
		return nullptr;
	}
	while (result->left) {
		result = result->left;
	}
	return result;
}

template <typename T, typename N>
N* BST<T, N>::getMaxNode(N* node) {
	N* result = node;
	if (!result) {
		return nullptr;
	}
	while (result->right) {
		result = result->right;
	}
	return result;
}

template <typename T, typename N>
void BST<T, N>::printTree(const std::string& prefix, N* node, bool isLeft)
{
	if (node)
	{
		if (node != root) {
			std::cout << prefix << (isLeft ? "L--" : "|>-");
		}
		node->print();

		if (node->parent) {
			std::cout << std::setw(10) << " " << "Parent: " << node->parent->value;
		}
		else {
			std::cout << std::setw(10) << " " << "Root";
		}

		std::cout << std::endl;

		if (node == root) {
			printTree(prefix + (!isLeft ? "" : ""), static_cast<N*>(node->right), false);
			printTree(prefix + (!isLeft ? "" : ""), static_cast<N*>(node->left), true);
		}
		else {
			printTree(prefix + (!isLeft ? "|   " : "    "), static_cast<N*>(node->right), false);
			printTree(prefix + (!isLeft ? "|   " : "    "), static_cast<N*>(node->left), true);
		}
	}
}

template <typename T, typename N>
N* BST<T, N>::leftRotate(N* node) {
	N* right = static_cast<N*>(node->right);

	assert(right);

	if (!right) {
		return nullptr;
	}

	node->right = right->left;
	if (node->right) {
		node->right->parent = node;
	}
	right->parent = node->parent;

	if (!node->parent) {
		root = right;
	}
	else {
		if (node->parent->left == node) {
			node->parent->left = right;
		}
		else {
			node->parent->right = right;
		}
	}

	right->left = node;
	right->left->parent = right;

	return right;
}

template <typename T, typename N>
N* BST<T, N>::rightRotate(N* node) {
	N* left = static_cast<N*>(node->left);

	assert(left);

	if (!left) {
		return nullptr;
	}

	node->left = left->right;
	if (node->left) {
		node->left->parent = node;
	}
	left->parent = node->parent;

	if (!node->parent) {
		root = left;
	}
	else {
		if (node->parent->right == node) {
			node->parent->right = left;
		}
		else {
			node->parent->left = left;
		}
	}

	left->right = node;
	left->right->parent = left;

	return left;
}

template <typename T, typename N>
void BST<T, N>::freeNode(N* node) {
	if (!node) return;
	freeNode(node->left);
	freeNode(node->right);
	delete node;
}

#endif // BST_HEADER_