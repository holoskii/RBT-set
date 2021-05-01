/* Set implemented as red-black tree build on top of binary search tree */

#ifndef RBT_HEADER_
#define RBT_HEADER_

#include "bst.h"

// Node for RBT holding one value
template <typename T>
class RBTNode {
public:
	RBTNode() {

	}

	RBTNode(T value) {
		this->value = value;
	}

	void print() {
		std::cout << std::setw(3) << std::left << this->value << std::setiosflags(std::ios::showbase) << " ";
		std::cout << (color ? "R" : "B") << " ";
	}

	T value;
	bool color = 1;
	RBTNode<T>* left = nullptr;
	RBTNode<T>* right = nullptr;
	RBTNode<T>* parent = nullptr;
};

template <typename T, typename N>
class RBTBaseSet : public BST<T, N>{
	using BST = BST<T, N>;
public:
	// Complements basic BST insertion with RBT balancing
	N* insert(T value);
	
	// Overwrites BST erase with RBT one
	void erase(T value);

	// RBT algorithm for node deletion
	N* deleteNode(N* v);
	
protected:
	static constexpr bool BLACK = 0;
	static constexpr bool RED = 1;

	// RBT balaning of given node
	void balance(N* node);

	// Double black case of balancing
	void fixDoubleBlack(N* node);
};

template <typename T, typename N>
N* RBTBaseSet<T,N>::insert(T value) {
	N* newNode = BST::find(BST::root, value);
	if (newNode) {
		return newNode;
	}
	BST::numElements++;
	newNode = new N(value);
	BST::root = BST::insertNode(BST::root, newNode);
	balance(newNode);
}

template <typename T, typename N>
void RBTBaseSet<T, N>::balance(N* node) {
	N* parent = nullptr;
	N* uncle = nullptr;
	N* grandparent = nullptr;


	while (node != BST::root && node->color == RED && node->parent->color == RED) {
		parent = node->parent;
		grandparent = node->parent->parent;

		if (grandparent->left == parent) {
			uncle = grandparent->right;
			if (uncle && uncle->color == RED) {
				grandparent->color = RED;
				parent->color = uncle->color = BLACK;
				node = grandparent;
			}
			else {
				if (parent->right == node) {
					BST::leftRotate(parent);
					node = parent;
					parent = parent->parent;
				}

				BST::rightRotate(grandparent);
				std::swap(parent->color, grandparent->color);
				node = parent;
			}
		}
		else {
			uncle = grandparent->left;
			if (uncle && uncle->color == RED) {
				grandparent->color = RED;
				parent->color = uncle->color = BLACK;
				node = grandparent;
			}
			else {
				if (parent->left == node) {
					BST::rightRotate(parent);
					node = parent;
					parent = parent->parent;
				}

				BST::leftRotate(grandparent);
				std::swap(parent->color, grandparent->color);
				node = parent;
			}
		}
	}
	BST::root->color = BLACK;
}

template <typename T, typename N>
void RBTBaseSet<T, N>::erase(T value) {
	N* node = BST::find(BST::root, value);
	if (!node) {
		return;
	}
	BST::numElements--;
	deleteNode(node);
}

template <typename T, typename N>
N* RBTBaseSet<T, N>::deleteNode(N* v) {
	// Ascend
	N* u = v;
	if (u->left && u->right) {
		u = u->right;
		while (u->left) {
			u = u->left;
		}
	}
	else if (!u->left && !u->right) {
		u = nullptr;
	}
	else if (u->left) {
		u = u->left;
	}
	else {
		u = u->right;
	}

	bool doubleBlack = (!u || u->color == BLACK) && (v->color == BLACK);

	N* parent = v->parent;
	if (!u) {
		if (v == BST::root) {
			BST::root = nullptr;
		}
		else {
			if (doubleBlack) {
				fixDoubleBlack(v);
			}
			else {
				// one black
				N* brother = nullptr;
				if (parent) {
					if (parent->left == v) {
						brother = parent->right;
					}
					else {
						brother = parent->left;
					}
				}
				if (brother) {
					brother->color = RED;
				}
			}

			// delete v
			if (parent->left == v) {
				parent->left = nullptr;
			}
			else {
				parent->right = nullptr;
			}
		}
		delete v;
		return u;
	}

	if (!v->left || !v->right) {
		if (v == BST::root) {
			v->value = u->value;
			v->left = v->right = nullptr;
			delete u;
			return nullptr;
		}
		else {
			if (parent->left == v) {
				parent->left = u;
			}
			else {
				parent->right = u;
			}
			u->parent = parent;
			delete v;
			if (doubleBlack) {
				fixDoubleBlack(u);
			}
			else {
				u->color = BLACK;
			}
			return u;
		}
	}

	std::swap(u->value, v->value);
	return deleteNode(u);
}

template <typename T, typename N>
void RBTBaseSet<T, N>::fixDoubleBlack(N* node) {
	if (!node) {
		return;
	}

	N* parent = node->parent;
	N* brother = nullptr;
	if (parent) {
		if (parent->left == node) {
			brother = parent->right;
		}
		else {
			brother = parent->left;
		}
	}

	if (!brother) {
		fixDoubleBlack(parent);
	}
	else {
		if (brother->color == RED) {
			parent->color = RED;
			brother->color = BLACK;

			if (parent->left == brother) {
				BST::rightRotate(parent);
			}
			else {
				BST::leftRotate(parent);
			}
			fixDoubleBlack(node);
		}
		else {
			// black brother
			if ((brother->left && brother->left->color == RED) || (brother->right && brother->right->color == RED)) {
				if (brother->left && brother->left->color == RED) {
					if (parent->left == brother) {
						brother->left->color = brother->color;
						brother->color = parent->color;
						BST::rightRotate(parent);
					}
					else {
						brother->left->color = parent->color;
						BST::rightRotate(brother);
						BST::leftRotate(parent);
					}
				}
				else {
					if (parent->left == brother) {
						brother->right->color = parent->color;
						BST::leftRotate(brother);
						BST::rightRotate(parent);
					}
					else {
						brother->right->color = brother->color;
						brother->color = parent->color;
						BST::leftRotate(parent);
					}
				}
				parent->color = BLACK;
			}
			else {
				brother->color = RED;
				if (parent->color == BLACK) {
					fixDoubleBlack(parent);
				}
				else {
					parent->color = BLACK;
				}
			}
		}
	}
}

#endif // RBT_HEADER_
