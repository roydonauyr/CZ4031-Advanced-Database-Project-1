#include <iostream>
#include <string>
//#include "storage.cpp"

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
	void insertKey(keyStruct key, Node* currNode, Node* child) {
		// case 1: no overflow
		if (currNode->size < MAX) {
			int i = 0, j;
	
			// Traverse the child node
			// for current cursor node
			while (key.value > currNode->key[i].value && i < currNode->size) {
				i++;
			}
	
			// Traverse the cursor node
			// and update the current key
			// to its previous node key
			for (j= currNode->size; j > i; j--) {
	
				currNode->key[j] = currNode->key[j - 1];
			}
	
			// Traverse the cursor node
			// and update the current ptr
			// to its previous node ptr
			for (j = currNode->size + 1; j > i + 1; j--) {
				currNode->ptr[j] = currNode->ptr[j - 1];
			}
	
			currNode->key[i] = key;
			currNode->size++;
			currNode->ptr[i + 1] = child;
		}
 
		// case 2: overflow, break the node
		else {
	
			// create new internal node
			Node* newInternal = new Node;
			keyStruct virtualKey[MAX + 1];
			Node* virtualPtr[MAX + 2];
			int i;
	
			// Insert the current list key of cursor node to virtualKey
			for (i = 0; i < MAX; i++) {
				virtualKey[i] = currNode->key[i];
			}
	
			// Insert the current list ptr of cursor node to virtualPtr
			for (i = 0; i < MAX + 1; i++) {
				virtualPtr[i] = currNode->ptr[i];
			}
	
			int i = 0, j;
	
			// Traverse to find where the new node is to be inserted
			while (key.value > virtualKey[i].value && i < MAX) {
				i++;
			}
	
			// Traverse the virtualKey node and update the current key to its previous node key
			for (j = MAX + 1; j > i; j--) {
				virtualKey[j] = virtualKey[j - 1];
			}
	
			virtualKey[i] = key;
	
			// Traverse the virtualKey node and update the current ptr to its previous node ptr
			for (j = MAX + 2; j > i + 1; j--) {
				virtualPtr[j] = virtualPtr[j - 1];
			}
	
			virtualPtr[i + 1] = child;
			newInternal->IS_LEAF = false;
	
			currNode->size = (MAX + 1) / 2;
	
			newInternal->size = MAX - (MAX + 1) / 2;
	
			// Insert new node as an internal node
			for (i = 0, j = currNode->size + 1; i < newInternal->size; i++, j++) {
				newInternal->key[i] = virtualKey[j];
			}
	
			for (i = 0, j = currNode->size + 1; i < newInternal->size + 1; i++, j++) {
				newInternal->ptr[i] = virtualPtr[j];
			}
	
			// If cursor is the root node
			if (currNode == root) {
	
				// Create a new root node
				Node* newRoot = new Node;
	
				// Update key value
				newRoot->key[0] = currNode->key[currNode->size];
	
				// Update rest field of B+ Tree Node
				newRoot->ptr[0] = currNode;
				newRoot->ptr[1] = newInternal;
				newRoot->IS_LEAF = false;
				newRoot->size = 1;
				root = newRoot;
			}
			else {
				// Recursive Call to insert the data
				insertInternal(currNode->key[currNode->size], findParent(root, currNode), newInternal);
			}
    	}
	}

	Node* findParent(Node* currNode, Node* child) {
        Node* parent;
        if(currNode->IS_LEAF || (currNode->ptr[0])->IS_LEAF) {
            return NULL;
        }
        for(int i = 0; i < currNode->size+1; i++) {
            if(currNode->ptr[i] == child) {
                parent = cursor;
                return parent;
            } else {
                parent = findParent(currNode->ptr[i],child);
                if(parent!=NULL)return parent;
            }
        }
        return parent;
    }

public:
	void insert(keyStruct key) {
		// If root is null then return newly created node
		if (root == NULL) {
			root = new Node;
			root->key[0] = key;
			root->IS_LEAF = true;
			root->size = 1;
		}
	
		// Traverse the B+ Tree
		else {
			Node* currNode = root;
			Node* parent;
	
			/////// search res?
			// Till cursor reaches the leaf node
			while (currNode->IS_LEAF == false) {
	
				parent = currNode;
	
				for (int i = 0; i < currNode->size; i++) {
					// If found the position where we have to insert node
					if (key < currNode->key[i]) {
						currNode = currNode->ptr[i];
						break;
					}
	
					// If reaches the end
					if (i == currNode->size - 1) {
						currNode = currNode->ptr[i + 1];
						break;
					}
				}
			}
	
			if (currNode->size < MAX) {
				int i = 0;
				while (key > currNode->key[i] && i < currNode->size) {
					i++;
				}
	
				for (int j = currNode->size; j > i; j--) {
					currNode->key[j] = currNode->key[j - 1];
				}

				currNode->key[i] = key;
				currNode->size++;
	
				currNode->ptr[currNode->size] = currNode->ptr[cursor->size - 1];
				currNode->ptr[currNode->size - 1] = NULL;
			}
	
			else {
				// Create a newLeaf node
				Node* newLeaf = new Node;
				keyStruct virtualNode[MAX + 1];
	
				// Update cursor to virtual node created
				for (int i = 0; i < MAX; i++) {
					virtualNode[i] = currNode->key[i];
				}
				int i = 0, j;
	
				// Traverse to find where the new node is to be inserted
				while (key.value > virtualNode[i].value && i < MAX) {
					i++;
				}
	
				// Update the current virtual Node to its previous
				for (j = MAX + 1; j > i; j--) {
					virtualNode[j] = virtualNode[j - 1];
				}
	
				virtualNode[i] = x;
				newLeaf->IS_LEAF = true;
	
				currNode->size = (MAX + 1) / 2;
				newLeaf->size = MAX + 1 - (MAX + 1) / 2;
	
				currNode->ptr[currNode->size] = newLeaf;
	
				newLeaf->ptr[newLeaf->size] = currNode->ptr[MAX];
	
				currNode->ptr[MAX] = NULL;
	
				// Update the current virtual node's key to its previous
				for (i = 0; i < currNode->size; i++) {
					currNode->key[i] = virtualNode[i];
				}
	
				// Update the newLeaf key to virtual Node
				for (i = 0, j = currNode->size; i < newLeaf->size; i++, j++) {
					newLeaf->key[i] = virtualNode[j];
				}
	
				// If cursor is the root node
				if (currNode == root) {
					// Create a new Node
					Node* newRoot = new Node;

					// Update rest field of
					// B+ Tree Node
					newRoot->key[0] = newLeaf->key[0];
					newRoot->ptr[0] = currNode;
					newRoot->ptr[1] = newLeaf;
					newRoot->IS_LEAF = false;
					newRoot->size = 1;
					root = newRoot;
				}
				else {
	
					// Recursive Call for insert in internal
					insertInternal(newLeaf->key[0], parent, newLeaf);
				}
			}
		}
	}

	// TODO: test this function
	void deleteKey(keyStruct key, Node* currNode, Node* child) {				// Set currNode and child to NULL on first run
		// Check if key is valid
		if (key == NULL) {
			cout << "Key is null." << endl;
			return;
		}

		if (currNode == root) {
			// Remove currNode entirely and set a child as root
			if (currNode->size == 1) {
				// Figure out which key the child is and delete the child
				if (currNode->ptr[0] == child) {
					root = currNode->ptr[0];
				}
				else if (currNode->ptr[1] == child) {
					root = currNode->ptr[1];
				}

				cout << "Root is too empty, changing root..."
				delete[] child->ptr;
				delete[] child->key;
				delete child;
			}
			// currNode has no children
			else if (currNode->size == 0) {
				cout << "No keys left in the tree. Killing tree..." << endl;
				root = NULL;
			}

			delete[] currNode->ptr;
			delete[] currNode->key;
			delete currNode;
			return;
		}
		else {
			Node* parent;						// Parent of currNode
			int leftSibling;					// Index of left sibling
			int rightSibling;					// Index of right sibling

			Node* cursor = root;
			// Loop through entire B+ tree until a leaf node is reached
			while (!(cursor->IS_LEAF)) {
				parent = cursor;
				leftSibling = i - 1;
				rightSibling = i + 1;

				// Find the node that might contain the key
				for (i = 0; i < cursor.size; i++) {
					if (key.value < cursor->key[i].value || i == cursor.size - 1) {
						// Go to child node
						cursor = cursor->ptr[i];
						if (currNode == NULL) {
							currNode = cursor;
						}
						break;
					}

				}
			}
			
			isDeleted = false;
			int index = 0;

			// Try to find key in currNode
			for (i = 0; i < currNode->size; i++) {
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

			// Case 1: check if no. of keys in currNode < floor((N + 1) / 2) 
			if (currNode->size >= floor((N + 1) / 2)) {
				deleteKey(key, parent, currNode);				// TODO: test if this is needed
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

				keyStruct unnecessaryKey = leftNode->key[leftNode->size - 1];

				// Transfer key from leftNode to currNode
				currNode->key[0] = leftNode->key[leftNode->size - 1];
				currNode->ptr[0] = leftNode->ptr[leftNode->size - 1];

				// Update leftNode's last pointer and remove last key
				leftNode->ptr[leftNode->size - 1] = leftNode->ptr[leftNode->size];
				leftNode->size--;

				// Update parent
				parent->key[leftSibling + 1] = cursor->key[0];
				deleteKey(unnecessaryKey, parent, currNode);
				return;
			}
			// Right sibling can make a transfer
			else if (rightNode != NULL && rightNode->size >= ceil((N + 1) / 2)) {
				// Make space for the transfer
				currNode->size++;

				// Transfer key from rightNode to currNode
				currNode->key[currNode->size - 1] = rightNode->key[0];
				currNode->ptr[currNode->size - 1] = rightNode->ptr[0];

				keyStruct unnecessaryKey = rightNode->key[0];

				// Remove rightNode's first key and update subsequent pointers and keys
				for (int i = 0; i < rightNode->size; i++) {
					rightNode->key[i] = rightNode->key[i + 1];
					rightNode->ptr[i] = rightNode->ptr[i + 1];
				}
				rightNode->ptr[rightNode->size] = rightNode->ptr[leftNode->size + 1];
				rightNode->size--;

				// Update parent
				parent->key[rightSibling] = rightNode->key[0];
				deleteKey(unnecessaryKey, parent, currNode);
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
	}

	// TODO
	void searchKeys() {

	}

	// TODO
	void searchRangeOfKeys() {

	}
};
