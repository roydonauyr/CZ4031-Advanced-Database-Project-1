#include <iostream>
#include <string>
#include "storage.cpp"

using namespace std;

// TODO: determine a good value for N
int N = 3;

// TODO
struct keyStruct {
	int value;
};

class Node {
	bool IS_LEAF;
	keyStruct* key;
	int size;								// In terms of keys
	Node** ptr;

public:
	Node()
	{
		// Dynamic memory allocation
		key = new keyStruct[N];				// Array of keys
		ptr = new Node * [N + 1];			// Array of pointers
		IS_LEAF = true;						// Set to false later during addition of new nodes
	}
};

// TODO
class BPTree {
	Node* root;

	// TODO
	void insertKey() {

	}

	// TODO
	void deleteParent(keyStruct key, Node* currNode, Node* child) {
		/*	if currNode == root
		*		if root only has 1 key
		*			if key is in left child
		*				delete child
		*				set right child as root
		*			else
		*				delete child
		*				set left child as root
		*
		*	// find the node that contains the key to be deleted
		*	for i from 0 to numOfLevels
		*		if key.value < currNode[0].value
		*			if currNode.IS_LEAF
		*				break
		*			go to left child
		*		else
		*			if currNode.IS_LEAF
		*				break
		*			go to right child
		*
		*	// iterate through the node and delete the key
		*	for i from 0 to nodeSize
		*		if currNode[i].value == key.value
		*			locate record
		*			delete record
		*			delete key from currNode
		*			break
		*
		*	// balance tree out so that it is a b+ tree
		*	if currNode is empty
		*		delete node
		*	elif currNode.size < floor(n/2) ??? // check if it is n/2 or (n+1)/2
		*		// case 2
		*		if sibling.size > floor(n/2)
		*			move a key from sibling to currNode
		*
		*		// case 3
		*		else
		*			if sibling is left
		*				merge with left sibling
		*				deleteParent(parent.key[leftSibling], parent, currNode)
		*			else
		*				merge with right sibling
		*				deleteParent(parent.key[rightSibling], parent, currNode)
		*/
	}

public:
	// TODO: test this function
	void deleteKey(keyStruct key) {
		// Check if key is valid
		if (key == NULL) {
			cout << "Key is null." << endl;
			return;
		}

		Node* currNode = root;
		Node* parent;						// Parent of currNode
		int leftSibling;					// Index of left sibling
		int rightSibling;					// Index of right sibling

		// Loop through entire B+ tree until a leaf node is reached
		while (!(currNode->IS_LEAF)) {
			parent = currNode;
			leftSibling = i - 1;
			rightSibling = i + 1;

			// Find the node that might contain the key
			for (i = 0; i < currNode.size; i++) {
				if (key.value < currNode->key[i].value || i == currNode.size - 1) {
					// Go to child node
					currNode = currNode->ptr[i];
					break;
				}

			}
		}

		isDeleted = false;
		int index = 0;

		for (i = 0; i < currNode->size; i++) {
			// TODO
			if (currNode->key[i].value == key.value) {
				isDeleted = true;
				index = i;
				cout << "Key found, deleting key..." << endl;
			}
		}

		// Key not in B+ tree
		if (!isDeleted) {
			cout << "Key not found." << endl;
			return;
		}

		// Delete key from node by moving subsequent keys and pointers 1 position forward
		for (int i = index; i < currNode->size; i++) {
			currNode->key[i] = currNode->key[i + 1];
			currNode->ptr[i] = currNode->ptr[i + 1];		// Not sure how this will go but should be correct
		}

		// Update last pointer in currNode and size of currNode
		currNode->ptr[currNode->size - 1] = currNode->ptr[currNode->size];
		currNode->size--;

		// Remove empty root
		if (currNode == root && currNode->size == 0) {
			cout << "No keys left in the tree. Killing tree..." << endl;
			delete[] currNode->key;
			delete[] currNode->ptr;
			delete currNode;
			root = NULL;
			return;
		}

		// Case 1: check if no. of keys in currNode < floor((N + 1) / 2) 
		if (currNode->size >= floor((N + 1) / 2)) {
			return;
		}

		Node* leftNode;
		Node* rightNode;

		// Case 2 and 3
		// Check if left sibling exists
		if (leftSibling >= 0) {
			leftNode = parent->ptr[leftSibling];
		}

		// Check if right sibling exists
		if (rightSibling <= N + 1) {
			rightNode = parent->ptr[rightSibling];
		}

		// Left sibling can make a transfer
		if (leftNode != NULL && leftNode->size >= ceil((N + 1) / 2)) {
			// Make space for the transfer
			currNode->size++;
			for (int i = currNode->size; i > 0; i--) {
				currNode->key[i - 1] = currNode->key[i - 2];
				currNode->ptr[i] = currNode->ptr[i - 1];
			}

			// Transfer key from leftNode to currNode
			currNode->key[0] = leftNode->key[leftNode->size - 1];
			currNode->ptr[0] = leftNode->ptr[leftNode->size - 1];

			// Update leftNode's last pointer and remove last key
			leftNode->ptr[leftNode->size - 1] = leftNode->ptr[leftNode->size];
			leftNode->size--;

			// Update parent
			parent->key[leftSibling + 1] = cursor->key[0];
			return;
		}
		// Right sibling can make a transfer
		else if (rightNode != NULL && rightNode->size >= ceil((N + 1) / 2)) {
			// Make space for the transfer
			currNode->size++;

			// Transfer key from rightNode to currNode
			currNode->key[currNode->size - 1] = rightNode->key[0];
			currNode->ptr[currNode->size - 1] = rightNode->ptr[0];

			// Remove rightNode's first key and update subsequent pointers and keys
			for (int i = 0; i < rightNode->size; i++) {
				rightNode->key[i] = rightNode->key[i + 1];
				rightNode->ptr[i] = rightNode->ptr[i + 1];
			}
			rightNode->ptr[rightNode->size] = rightNode->ptr[leftNode->size + 1];
			rightNode->size--;

			// Update parent
			parent->key[rightSibling] = rightNode->key[0];
			return;
		}
		// Merge leftNode and currNode
		else if (leftNode != NULL) {
			// Transfer all keys and pointers from currNode to leftNode
			leftNode->size += currNode->size;
			for (int i = leftNode->size, j = 0; j < currNode->size; i++, j++) {
				leftNode->key[i] = currNode->key[j];
				leftNode->ptr[i + 1] = currNode->ptr[j];
			}
			leftNode->ptr[leftNode->size] = currNode->ptr[leftNode->size];

			// Delete currNode from parent
			deleteParent(parent->key[leftSibling + 1], parent, currNode);
		}
		// Merge with rightNode
		else {
			// Transfer all keys and pointers from rightNode to currNode
			currNode->size += rightNode->size;
			for (int i = currNode->size, j = 0; j < rightNode->size; i++, j++) {
				currNode->key[i] = rightNode->key[j];
				currNode->ptr[i + 1] = rightNode->ptr[j];
			}
			currNode->ptr[currNode->size] = rightNode->ptr[rightNode->size];

			// Delete rightNode from parent
			deleteParent(parent->key[rightSibling], parent, rightNode);
		}

		// Delete currNode
		cout << "Deleting node..." << endl;
		delete[] currNode->key;
		delete[] currNode->ptr;
		delete currNode;
		return;
	}

	// TODO
	void searchKeys() {

	}

	// TODO
	void searchRangeOfKeys() {

	}
};
