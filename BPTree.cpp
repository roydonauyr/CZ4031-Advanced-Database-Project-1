#include <iostream>
#include <string>
//#include "storage.cpp"

using namespace std;

// TODO: determine a good value for N
int N = 3;

// TODO
struct keyStruct
{
	int value;
};

class Node
{
	bool IS_LEAF;
	keyStruct *key;
	int size; // In terms of keys
	Node **ptr;

public:
	Node()
	{
		// Dynamic memory allocation
		key = new keyStruct[N];	 // Array of keys
		ptr = new Node *[N + 1]; // Array of pointers
		IS_LEAF = true;			 // Set to false later during addition of new nodes
	}
};

// TODO
class BPTree
{
	Node *root;

	// TODO
	void insertInternal(keyStruct key, Node *currNode, Node *child)
	{
		// if parent (current node) still has space, we can add the child node as a pointer
		if (currNode->size < max keys) {
			int i = 0, j;

			// iterate through the parent to see where to put in the lower bound key for the new child
			while (key.value > currNode->key[i].value && i < currNode->size) {
				i++;
			}

			// using index i, move the larger keys backwards
			for (j = currNode->size; j > i; j--) {
				currNode->key[j] = currNode->key[j - 1];
			}

			// shift all pointers right
			for (j = currNode->size + 1; j > i + 1; j--) {
				currNode->ptr[j] = currNode->ptr[j - 1];
			}

			// add in new child's lower bound key and pointer to the parent
			currNode->key[i] = key;
			currNode->size++;
			// rightmost pointer will point to new child node
			currNode->ptr[i + 1] = child;
		}

		// if parent node no space, recursively split parent node and insert more parent nodes
		else {
			// TODO: create new internal node
			/*
			Node *newInternal = new Node;
			*/

			// create new templist to store the keys
			keyStruct virtualKey[MAX + 1];
			Node *virtualPtr[MAX + 2];

			// copy current list of keys to temp key list
			for (int i = 0; i < max keys; i++) {
				virtualKey[i] = currNode->key[i];
			}

			// copy all pointers to temp pointer list
			// there is one more pointer than keys, so maxKeys + 1.
			for (int i = 0; i < max keys + 1; i++) {
				virtualPtr[i] = currNode->ptr[i];
			}

			int i = 0, j;

			// traverse to find where the new node is to be inserted in temp key list
			while (key.value > virtualKey[i].value && i < max keys) {
				i++;
			}

			// move keys backwards to make space for insertion
			for (j = max keys + 1; j > i; j--)
			{
				virtualKey[j] = virtualKey[j - 1];
			}

			// insert new key into temp key list
			virtualKey[i] = key;

			// move ptrs backwards
			for (j = max keys + 2; j > i + 1; j--) {
				virtualPtr[j] = virtualPtr[j - 1];
			}

			// insert a ptr to the child to the right of its key
			virtualPtr[i + 1] = child;
			newInternal->IS_LEAF = false;

			// split new node into 2
			currNode->size = (max keys + 1) / 2;
			newInternal->size = max keys - (max keys + 1) / 2;

			// insert new keys into new internal parent node from temp key list
			for (i = 0, j = currNode->size + 1; i < newInternal->size; i++, j++) {
				newInternal->key[i] = virtualKey[j];
			}

			// insert new ptrs into new internal parent node from temp ptr list
			for (i = 0, j = currNode->size + 1; i < newInternal->size + 1; i++, j++) {
				newInternal->ptr[i] = virtualPtr[j];
			}

			// if current node is the root node, create a new root node
			if (currNode == root) {

				// TODO: create a new root node
				/*
				Node *newRoot = new Node;
				*/

				// use rightmost key of old parent to be root
				newRoot->key[0] = currNode->key[currNode->size];

				// update 2 children
				newRoot->ptr[0] = currNode;
				newRoot->ptr[1] = newInternal;
				// set other fields
				newRoot->IS_LEAF = false;
				newRoot->size = 1;

				root = newRoot;
			}
			// if parent is internal, need to split and make another new parent
			else {
				// recursive Call to insert the data
				insertInternal(currNode->key[currNode->size], findParent(root, currNode), newInternal);
			}
		}
	}

	// finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
	Node *findParent(Node *currNode, Node *child)
	{
		Node *parent;

		// leaf node == not a parent 
		if (currNode->IS_LEAF || (currNode->ptr[0])->IS_LEAF) {
			return NULL;
		}

		for (int i = 0; i < currNode->size + 1; i++) {
			if (currNode->ptr[i] == child) {
				parent = currNode;
				return parent;
			}
			else {
				parent = findParent(currNode->ptr[i], child);
				if (parent != NULL) {
					return parent;
				}
			}
		}
		return parent;
	}

public:
	void insert(keyStruct key)
	{
		// if there is no root, create new B+ tree root
		if (root == NULL) {
			// TODO: create a new node 
			// root == new node
			/*
			root = new Node;
			root->key[0] = key;
			root->IS_LEAF = true;
			root->size = 1; 
			*/
		}

		// if there is already a root node, traverse the b+ tree to see where to insert
		else {
			Node *currNode = root;
			Node *parent;

			// while not leaf, traverse the tree to correct key
			while (currNode->IS_LEAF == false) {
				parent = currNode;

				// check through all keys of the node to find key and pointer to follow downwards
				for (int i = 0; i < currNode->size; i++) {
					// if found correct position to insert node
					if (key < currNode->key[i]) {
						currNode = currNode->ptr[i];
						break;
					}

					// if reached the end, aka key is larger than all keys in the node
					if (i == currNode->size - 1) {
						currNode = currNode->ptr[i + 1];
						break;
					}
				}
			}

			// now at a leaf node where we want to insert the key
			// if the leaf node still has capacity for more keys
			if (currNode->size < max num of keys) {
				int i = 0;

				// while we haven't reached the last key and the key we want to insert is larger than current key
				while (key > currNode->key[i] && i < currNode->size) {
					i++;
				}

				// shift all the keys back to insert the current key
				// i is the index we want to put the key in
				for (int j = currNode->size; j > i; j--) {
					currNode->key[j] = currNode->key[j - 1];
				}

				// insert new key here
				currNode->key[i] = key;
				// increment size of node inserted into
				currNode->size++;

				// adjust pointer to next leaf node to be on the last index, previously was on second last
				// set second last index as NULL
				currNode->ptr[currNode->size] = currNode->ptr[currNode->size - 1];
				currNode->ptr[currNode->size - 1] = NULL;
			}

			// overflow: no space to insert new key, so we have to split this node into two 
			else {
				// TODO: create a newLeaf node
				/*
				Node *newLeaf = new Node;
				*/

				//create temp list
				keyStruct virtualNode[MAX + 1];

				// update cursor to virtual node created
				// copy all keys to temp list
				for (int i = 0; i < MAX; i++) {
					virtualNode[i] = currNode->key[i];
				}
				int i = 0, j;

				// traverse to find where the new node is to be inserted, inside the temp list
				while (key.value > virtualNode[i].value && i < max num of keys) {
					i++;
				}

				// move other keys larger than the current key backwards to make space
				for (j = max keys + 1; j > i; j--) {
					virtualNode[j] = virtualNode[j - 1];
				}

				// insert the key into the temp list
				virtualNode[i] = key;

				// mark the new node created newLeaf as a leaf node
				newLeaf->IS_LEAF = true;

				// split node into 2
				// current node will be start of left half, new node will be start of right half
				currNode->size = (max keys + 1) / 2;
				newLeaf->size = max keys + 1 - (max keys + 1) / 2;

				// set pointer of current node to point to new node
				currNode->ptr[currNode->size] = newLeaf;

				// set pointer of new node to point to next node
				newLeaf->ptr[newLeaf->size] = currNode->ptr[max keys];
				currNode->ptr[MAX] = NULL;

				// add in the keys to the current node from the templist
				for (i = 0; i < currNode->size; i++) {
					currNode->key[i] = virtualNode[i];
				}

				// add in the keys to the new node from the templist
				// need to keep track of index j since using remaining keys from current node
				for (i = 0, j = currNode->size; i < newLeaf->size; i++, j++) {
					newLeaf->key[i] = virtualNode[j];
				}

				// if we are at root (root == leaf), we need to make a new parent root
				if (currNode == root) {
					// TODO: create a new Node for root
					/*
					Node *newRoot = new Node;
					*/

					// update the rest of the B+ tree
					// set new root's key to be the left bound of the right child
					newRoot->key[0] = newLeaf->key[0];
					// set pointers
					newRoot->ptr[0] = currNode;
					newRoot->ptr[1] = newLeaf;
					// set other fields
					newRoot->IS_LEAF = false;
					newRoot->size = 1;
					root = newRoot;
				}
				// if it is not the root, insert a new parent in the internal nodes of the tree
				else {

					// Recursive Call for insert in internal
					insertInternal(newLeaf->key[0], parent, newLeaf);
				}
			}
		}
	}

	// TODO: test this function
	void deleteKey(keyStruct key, Node *currNode, Node *child)
	{ // Set currNode and child to NULL on first run
		// Check if key is valid
		if (key == NULL)
		{
			cout << "Key is null." << endl;
			return;
		}

		if (currNode == root)
		{
			// Remove currNode entirely and set a child as root
			if (currNode->size == 1)
			{
				// Figure out which key the child is and delete the child
				if (currNode->ptr[0] == child)
				{
					root = currNode->ptr[0];
				}
				else if (currNode->ptr[1] == child)
				{
					root = currNode->ptr[1];
				}

				cout << "Root is too empty, changing root..." delete[] child->ptr;
				delete[] child->key;
				delete child;
			}
			// currNode has no children
			else if (currNode->size == 0)
			{
				cout << "No keys left in the tree. Killing tree..." << endl;
				root = NULL;
			}

			delete[] currNode->ptr;
			delete[] currNode->key;
			delete currNode;
			return;
		}
		else
		{
			Node *parent;	  // Parent of currNode
			int leftSibling;  // Index of left sibling
			int rightSibling; // Index of right sibling

			Node *cursor = root;
			// Loop through entire B+ tree until a leaf node is reached
			while (!(cursor->IS_LEAF))
			{
				parent = cursor;
				leftSibling = i - 1;
				rightSibling = i + 1;

				// Find the node that might contain the key
				for (i = 0; i < cursor.size; i++)
				{
					if (key.value < cursor->key[i].value || i == cursor.size - 1)
					{
						// Go to child node
						cursor = cursor->ptr[i];
						if (currNode == NULL)
						{
							currNode = cursor;
						}
						break;
					}
				}
			}

			isDeleted = false;
			int index = 0;

			// Try to find key in currNode
			for (i = 0; i < currNode->size; i++)
			{
				if (currNode->key[i].value == key.value)
				{
					isDeleted = true;
					index = i;
					cout << "Key found, deleting key..." << endl;
				}
			}

			// Key not in B+ tree
			if (!isDeleted)
			{
				cout << "Key not found." << endl;
				return;
			}

			// Delete key from node by moving subsequent keys and pointers 1 position forward
			for (int i = index; i < currNode->size; i++)
			{
				currNode->key[i] = currNode->key[i + 1];
				currNode->ptr[i] = currNode->ptr[i + 1]; // Not sure how this will go but should be correct
			}

			// Update last pointer in currNode and size of currNode
			currNode->ptr[currNode->size - 1] = currNode->ptr[currNode->size];
			currNode->size--;

			// Case 1: check if no. of keys in currNode < floor((N + 1) / 2)
			if (currNode->size >= floor((N + 1) / 2))
			{
				deleteKey(key, parent, currNode); // TODO: test if this is needed
				return;
			}

			Node *leftNode;
			Node *rightNode;

			// Case 2 and 3
			// Check if left sibling exists
			if (leftSibling >= 0)
			{
				leftNode = parent->ptr[leftSibling];
			}

			// Check if right sibling exists
			if (rightSibling <= N + 1)
			{
				rightNode = parent->ptr[rightSibling];
			}

			// Left sibling can make a transfer
			if (leftNode != NULL && leftNode->size >= ceil((N + 1) / 2))
			{
				// Make space for the transfer
				currNode->size++;
				for (int i = currNode->size; i > 0; i--)
				{
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
			else if (rightNode != NULL && rightNode->size >= ceil((N + 1) / 2))
			{
				// Make space for the transfer
				currNode->size++;

				// Transfer key from rightNode to currNode
				currNode->key[currNode->size - 1] = rightNode->key[0];
				currNode->ptr[currNode->size - 1] = rightNode->ptr[0];

				keyStruct unnecessaryKey = rightNode->key[0];

				// Remove rightNode's first key and update subsequent pointers and keys
				for (int i = 0; i < rightNode->size; i++)
				{
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
			else if (leftNode != NULL)
			{
				// Transfer all keys and pointers from currNode to leftNode
				leftNode->size += currNode->size;
				for (int i = leftNode->size, j = 0; j < currNode->size; i++, j++)
				{
					leftNode->key[i] = currNode->key[j];
					leftNode->ptr[i + 1] = currNode->ptr[j];
				}
				leftNode->ptr[leftNode->size] = currNode->ptr[leftNode->size];

				// Delete currNode from parent
				deleteParent(parent->key[leftSibling + 1], parent, currNode);
			}
			// Merge with rightNode
			else
			{
				// Transfer all keys and pointers from rightNode to currNode
				currNode->size += rightNode->size;
				for (int i = currNode->size, j = 0; j < rightNode->size; i++, j++)
				{
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
	void searchKeys(int x)
	{
		if (root == NULL)
		{
			cout << "Tree is empty\n";
		}
		else
		{
			Node *cursor = root;
			// until you find a leaf node
			while (cursor->IS_LEAF == false)
			{
				for (int i = 0; i < cursor->size; i++)
				{
					if (x < cursor->key[i])
					{
						cursor = cursor->ptr[i];
						break;
					}
					if (i == (cursor->size) - 1)
					{
						cursor = cursor->ptr[i + 1];
						break;
					}
				}
			}
			for (int i = 0; i < cursor->size; i++)
			{
				if (cursor->key[i] == x)
				{
					// Print out key details as well
					cout << "Found\n";
					return;
				}
			}
			cout << "Not found\n";
			// send not found not sure if need to return nullptr
		}
	}

	// TODO
	void searchRangeOfKeys(int LowerBound, int UpperBound)
	{
		// search logic
		if (root == NULL)
		{
			// empty
			// cout<<"Tree empty\n";
		}
		else
		{
			CurPtr = rootNode;
			// in the following while loop, cursor will travel to the leaf node possibly consisting the key
			while (CurPtr != Leaf_Node)
			{
				// use the totalSizeDB as ending point of loop
				for (int i = 0; i < TotalSizeDB; i++)
				{
					if (LowerBound < CurPtr->key[i] && UpperBound <= CurPtr->key[i])
					{
						CurPtr = CurPtr->ptr[i];
						// cout<<"Found\n";
						// print values of the CurPtr experiemnt 3 requires
						// the number and the content of data blocks the process accesses
						// the average of “averageRating’s” of the records that are returned
						break;
					}
					// if (i == CurPtr->size - 1)
					// {
					//     CurPtr = CurPtr->ptr[i + 1];
					//     break;
					// }
				}
			}

			print("NotFound")
			// cout<<"Not found\n";
		}
	}
};
