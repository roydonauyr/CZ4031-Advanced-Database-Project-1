#include <iostream>
#include <string>
#include "Database_Storage.hpp"

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
	BlockManager *blkManager;
	
	unsigned int rootNode; //Stores index of root node

	BPTree(BlockManager *mgr){
		this->blkManager = mgr;
	}
	// TODO
	void insertInternal(keyStruct key, Node *currNode, Node *child)
	{
		// if parent (current node) still has space, we can add the child node as a pointer
		if (currNode->size < max keys)
		{
			int i = 0, j;

			// iterate through the parent to see where to put in the lower bound key for the new child
			while (key.value > currNode->key[i].value && i < currNode->size)
			{
				i++;
			}

			// using index i, move the larger keys backwards
			for (j = currNode->size; j > i; j--)
			{
				currNode->key[j] = currNode->key[j - 1];
			}

			// shift all pointers right
			for (j = currNode->size + 1; j > i + 1; j--)
			{
				currNode->ptr[j] = currNode->ptr[j - 1];
			}

			// add in new child's lower bound key and pointer to the parent
			currNode->key[i] = key;
			currNode->size++;
			// rightmost pointer will point to new child node
			currNode->ptr[i + 1] = child;
		}

		// if parent node no space, recursively split parent node and insert more parent nodes
		else
		{
			// TODO: create new internal node
			/*
			Node *newInternal = new Node;
			*/

			// create new templist to store the keys
			keyStruct virtualKey[MAX + 1];
			Node *virtualPtr[MAX + 2];

			// copy current list of keys to temp key list
			for (int i = 0; i < max keys; i++)
			{
				virtualKey[i] = currNode->key[i];
			}

			// copy all pointers to temp pointer list
			// there is one more pointer than keys, so maxKeys + 1.
			for (int i = 0; i < max keys + 1; i++)
			{
				virtualPtr[i] = currNode->ptr[i];
			}

			int i = 0, j;

			// traverse to find where the new node is to be inserted in temp key list
			while (key.value > virtualKey[i].value && i < max keys)
			{
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
			for (j = max keys + 2; j > i + 1; j--)
			{
				virtualPtr[j] = virtualPtr[j - 1];
			}

			// insert a ptr to the child to the right of its key
			virtualPtr[i + 1] = child;
			newInternal->IS_LEAF = false;

			// split new node into 2
			currNode->size = (max keys + 1) / 2;
			newInternal->size = max keys - (max keys + 1) / 2;

			// insert new keys into new internal parent node from temp key list
			for (i = 0, j = currNode->size + 1; i < newInternal->size; i++, j++)
			{
				newInternal->key[i] = virtualKey[j];
			}

			// insert new ptrs into new internal parent node from temp ptr list
			for (i = 0, j = currNode->size + 1; i < newInternal->size + 1; i++, j++)
			{
				newInternal->ptr[i] = virtualPtr[j];
			}

			// if current node is the root node, create a new root node
			if (currNode == root)
			{

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
			else
			{
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
		if (currNode->IS_LEAF || (currNode->ptr[0])->IS_LEAF)
		{
			return NULL;
		}

		for (int i = 0; i < currNode->size + 1; i++)
		{
			if (currNode->ptr[i] == child)
			{
				parent = currNode;
				return parent;
			}
			else
			{
				parent = findParent(currNode->ptr[i], child);
				if (parent != NULL)
				{
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
		if (root == NULL)
		{
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
		else
		{
			Node *currNode = root;
			Node *parent;

			// while not leaf, traverse the tree to correct key
			while (currNode->IS_LEAF == false)
			{
				parent = currNode;

				// check through all keys of the node to find key and pointer to follow downwards
				for (int i = 0; i < currNode->size; i++)
				{
					// if found correct position to insert node
					if (key < currNode->key[i])
					{
						currNode = currNode->ptr[i];
						break;
					}

					// if reached the end, aka key is larger than all keys in the node
					if (i == currNode->size - 1)
					{
						currNode = currNode->ptr[i + 1];
						break;
					}
				}
			}

			// now at a leaf node where we want to insert the key
			// if the leaf node still has capacity for more keys
			if (currNode->size < max num of keys)
			{
				int i = 0;

				// while we haven't reached the last key and the key we want to insert is larger than current key
				while (key > currNode->key[i] && i < currNode->size)
				{
					i++;
				}

				// shift all the keys back to insert the current key
				// i is the index we want to put the key in
				for (int j = currNode->size; j > i; j--)
				{
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
			else
			{
				// TODO: create a newLeaf node
				/*
				Node *newLeaf = new Node;
				*/

				// create temp list
				keyStruct virtualNode[MAX + 1];

				// update cursor to virtual node created
				// copy all keys to temp list
				for (int i = 0; i < MAX; i++)
				{
					virtualNode[i] = currNode->key[i];
				}
				int i = 0, j;

				// traverse to find where the new node is to be inserted, inside the temp list
				while (key.value > virtualNode[i].value && i < max num of keys)
				{
					i++;
				}

				// move other keys larger than the current key backwards to make space
				for (j = max keys + 1; j > i; j--)
				{
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
				for (i = 0; i < currNode->size; i++)
				{
					currNode->key[i] = virtualNode[i];
				}

				// add in the keys to the new node from the templist
				// need to keep track of index j since using remaining keys from current node
				for (i = 0, j = currNode->size; i < newLeaf->size; i++, j++)
				{
					newLeaf->key[i] = virtualNode[j];
				}

				// if we are at root (root == leaf), we need to make a new parent root
				if (currNode == root)
				{
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
				else
				{

					// Recursive Call for insert in internal
					insertInternal(newLeaf->key[0], parent, newLeaf);
				}
			}
		}
	}

	// TODO: test this function
	void deleteKey(int key, unsigned int curr, unsigned int child)
	{
		// Set currNode and child to NULL on first run
		// if key is not valid, return
		//
		// if currNode is root
		//	if there is only 1 key in the root
		//		set 1 of the children (key != key to be deleted) as root
		//		delete old root (i.e. currNode)
		//	if root is empty
		//		delete root
		// else if currNode is null
		//	for each non-leaf node in B+ tree
		//		record parent, leftSibling and rightSibling of currNode
		//		for each key[i] in node
		//			if key < key[i] (i.e. key potentially found)
		//				go to child node via ith pointer
		//				break
		//	instantiate deleted = false and index = 0 (to check if key is found and the key's index within the node)
		//	for each key in currNode (currNode is a leaf node)
		//		if key == key in leaf node
		//			deleted = false
		//			index = key's current index
		//	if deleted = false (i.e. key not found)
		//		return
		//	for each key in currNode after index
		//		move pointer and key 1 index forward
		// else
		//	if currNode is leaf && remaining keys >= floor((n + 1) / 2) || currNode is not leaf && remaining keys >= floor(n / 2) (i.e. case 1)
		// 		delete(key, parent) (delete key from parent if necessary)
		//		return
		//	else
		//		instantiate leftNode, rightNode and redundantKey
		//		if leftSibling exists
		//			leftNode = node at leftSibling
		//		if rightSibling exists
		//			rightNode = node at rightSibling
		//		if (leftNode is leaf && keys >= floor((n + 1) / 2)) || (leftNode is not leaf && keys >= floor(n / 2)) (case 2)
		//			create space for transfer (if needed)
		//			transfer a key from leftNode to currNode
		//			redundantKey = transferred key
		//			delete redundantKey in leftNode
		//			delete(redundantKey, parent)
		//			return
		//		else if (leftNode is leaf && keys >= floor((n + 1) / 2)) || (leftNode is not leaf && keys >= floor(n / 2)) (case 2)
		//			create space for transfer (if needed)
		//			transfer a key from rightNode to currNode
		//			redundantKey = transferred key
		//			delete redundantKey in rightNode
		//			delete(redundantKey, parent)
		//			return
		//		else (case 3)
		//			if leftNode != null (i.e. leftNode exists)
		//				merge leftNode and currNode
		//				delete currNode from parent
		//			else if rightNode != null (i.e. rightNode exists)
		//				merge currNode and rightNode
		//				delete currNode from parent
		//			delete currNode
		//			delete(key, parent)
		//			return


		


		// Check if key is valid
		if (key == NULL) {
			cout << "Key is null." << endl;
			return;
		}

		treeNodeBlock *currNode = static_cast<treeNodeBlock*>(blkManager->accessBlock(curr));

		// If currNode is root
		if (curr == rootNode) {
			// Remove currNode entirely and set a child as root
			if (currNode->getLength() == 1) {
				if (currNode->ptrs[0].getBlock() == child) {
					rootNode = currNode->ptrs[0].getBlock();
				}
				else if (currNode->ptrs[1].getBlock() == child) {
					rootNode = currNode->ptrs[1].getBlock();
				}

				cout << "Root is too empty, changing root..." << endl;
				blkManager->deleteBlock(child);
			}
			else if (currNode->getLength() == 0) {
				cout << "No keys left in the tree. Killing tree..." << endl;
				rootNode = NULL;
			}

			blkManager->deleteBlock(curr);
			return;
		}
		else {
			int leftSibling;			// Index of left sibling
			int rightSibling;			// Index of right sibling

			treeNodeBlock *cursor = static_cast<treeNodeBlock*>(blkManager->accessBlock(rootNode));

			// Loop through entire B+ tree until a leaf node is reached
			while (cursor->type != (unsigned char) "2") {
				int index = 0;
				// Find the node that might contain the key
				for (int i = 0; i < cursor->getLength(); i++) {
					if (key < cursor->key[i] || i == cursor->getLength()) {
						// Go to child node
						cursor = static_cast<treeNodeBlock*>(blkManager->accessBlock(cursor->ptrs[i].getBlock()));
						if (currNode == NULL) {
							currNode = dynamic_cast<treeNodeBlock*>(cursor);
						}
						break;
					}
				}
				leftSibling = index - 1;
				rightSibling = index + 1;
			}

			treeNodeBlock* parent = static_cast<treeNodeBlock*>(blkManager->accessBlock(cursor->getParentBlock()));		// Parent of currNode

			bool isDeleted = false;
			int index = 0;

			// Try to find key in currNode
			for (int i = 0; i < currNode->getLength(); i++) {
				if (currNode->key[i] == key) {
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

			// TODO: Delete linked list
			if (blkManager->accessBlock(currNode->ptrs[index].getBlock())->type == '2') {
				linkedListNodeBlock *toBeDeleted = static_cast<linkedListNodeBlock*>(blkManager->accessBlock(currNode->ptrs[index].getBlock()));

				while (toBeDeleted->nextBlock != nullptr) {
					linkedListNodeBlock *currDelete = toBeDeleted;
					toBeDeleted = static_cast<linkedListNodeBlock*>(blkManager->accessBlock(toBeDeleted->nextBlock.getBlock()));
					blkManager->deleteBlock(currDelete->nextBlock.getBlock());
				}
			}
			else if (blkManager->accessBlock(currNode->ptrs[index].getBlock())->type == '0') {
				RecordBlock *toBeDeleted = static_cast<RecordBlock*>(blkManager->accessBlock(currNode->ptrs[index].getBlock()));
				blkManager->deleteBlock(currNode->ptrs[index].getBlock());
			}

			// Delete key
			currNode->key[index] = 0;
			currNode->ptrs[index].entry = (char) "0";

			// Move subsequent keys and pointers 1 position forward
			for (int i = index; i < currNode->getLength(); i++) {
				currNode->key[i] = currNode->key[i + 1];
				currNode->ptrs[i] = currNode->ptrs[i + 1]; // Not sure how this will go but should be correct
			}

			// Update last pointer in currNode
			currNode->ptrs[currNode->getLength() - 1] = currNode->ptrs[currNode->getLength()];

			// Case 2 and 3
			treeNodeBlock *leftNode;
			treeNodeBlock *rightNode;

			// Check if left sibling exists
			if (leftSibling >= 0) {
				leftNode = static_cast<treeNodeBlock*>(blkManager->accessBlock(parent->ptrs[leftSibling].getBlock()));
			}

			// Check if right sibling exists
			if (rightSibling <= N + 1) {
				rightNode = static_cast<treeNodeBlock*>(blkManager->accessBlock(parent->ptrs[rightSibling].getBlock()));
			}

			// Left sibling can make a transfer
			if (leftNode != NULL && leftNode->getLength() >= ceil((NUM_KEY_INDEX + 1) / 2)) {
				// Make space for the transfer
				for (int i = currNode->getLength(); i > 0; i--) {
					currNode->key[i - 1] = currNode->key[i - 2];
					currNode->ptrs[i] = currNode->ptrs[i - 1];
				}

				unsigned int unnecessaryKey = leftNode->key[leftNode->getLength() - 1];

				// Transfer key from leftNode to currNode
				currNode->key[0] = leftNode->key[leftNode->getLength() - 1];
				currNode->ptrs[0] = leftNode->ptrs[leftNode->getLength() - 1];

				// Update leftNode's last pointer and remove last key
				leftNode->ptrs[leftNode->getLength() - 1] = leftNode->ptrs[leftNode->getLength()];

				// Update parent
				parent->key[leftSibling + 1] = cursor->key[0];
				deleteKey(unnecessaryKey, currNode->getParentBlock(), curr);
				return;
				}
			// Right sibling can make a transfer
			else if (rightNode != NULL && rightNode->getLength() >= ceil((N + 1) / 2)) {			
				// Transfer key from rightNode to currNode
				currNode->key[currNode->getLength() - 1] = rightNode->key[0];
				currNode->ptrs[currNode->getLength() - 1] = rightNode->ptrs[0];

				unsigned int unnecessaryKey = leftNode->key[rightNode->getLength() - 1];

				// Remove rightNode's first key and update subsequent pointers and keys
				for (int i = 0; i < rightNode->getLength(); i++) {
					rightNode->key[i] = rightNode->key[i + 1];
					rightNode->ptrs[i] = rightNode->ptrs[i + 1];
				}
				rightNode->ptrs[rightNode->getLength()] = rightNode->ptrs[leftNode->getLength() + 1];

				// Update parent
				parent->key[rightSibling] = rightNode->key[0];
				deleteKey(unnecessaryKey, currNode->getParentBlock(), curr);
				return;
			}

			// Merge leftNode and currNode
			else if (leftNode != NULL)
			{
				// Transfer all keys and pointers from currNode to leftNode
				for (int i = leftNode->getLength(), j = 0; j < currNode->getLength(); i++, j++)
				{
					leftNode->key[i] = currNode->key[j];
					leftNode->ptrs[i + 1] = currNode->ptrs[j];
				}
				leftNode->ptrs[leftNode->getLength()] = currNode->ptrs[leftNode->getLength()];

				// Delete currNode from parent
				deleteKey(parent->key[leftSibling + 1], currNode->getParentBlock(), parent->ptrs[leftSibling].getBlock());
			}
			// Merge with rightNode
			else
			{
				// Transfer all keys and pointers from rightNode to currNode
				for (int i = currNode->getLength(), j = 0; j < rightNode->getLength(); i++, j++)
				{
					currNode->key[i] = rightNode->key[j];
					currNode->ptrs[i + 1] = rightNode->ptrs[j];
				}
				currNode->ptrs[currNode->getLength()] = rightNode->ptrs[rightNode->getLength()];

				// Delete rightNode from parent
				deleteKey(parent->key[rightSibling], currNode->getParentBlock(), parent->ptrs[rightSibling].getBlock());
			}

		}
		
		// Delete currNode
		cout << "Deleting node..." << endl;
		blkManager->deleteBlock(curr);
		return;
	}

	// If There is multiple entries then print only the first five
	void searchKeys(int key)
	{
		if(root == nullptr) {
			cout << "Tree is empty\n" << endl;
		} else {

		}
		
		if (root == NULL)
		{
			cout << "Tree is empty\n";
		}
		else
		{
			Node *cursor = root;
			// While loop until you find a leaf node
			while (cursor->IS_LEAF == false)
			{
				// Apply binary search and once find the
				for (int i = 0; i < cursor->size; i++)
				{
					if (key < cursor->item[i])
					{
						// find child node
						cursor = cursor->children[i];
						break;
					}
					if (i == (cursor->size) - 1)
					{
						cursor = cursor->children[i + 1];
						break;
					}
				}
			}
			for (int i = 0; i < cursor->size; i++)
			{
				if (cursor->item[i] == key)
				{
					// Print out key details as well
					// Check next children whether its the same key
					// If key is the same need to print also
					// Total five times if more than five
					// Another While loop
					return;
				}
			}
			cout << "Not found\n";
			// send not found not sure if need to return nullptr
		}
	}

	// TODO
	// **RecordBlock i think?
	void searchRangeOfKeys(int LowerBound, int UpperBound)
	{

		block *cursor;
		if(root != NULL) {
			if(cursor->type == 0) {
				RecordBlock *curr = (RecordBlock *) cursor;
				for(int i = 0; i < 12; i++) {
					if(curr->records[i] != nullptr) {
						cout << curr->records[i].avgRating << endl;
					}
				}
			} else if (cursor->type == 1) {
				treeNodeBlock *curr = (treeNodeBlock *)cursor;
			}
			int size = root->getLength();

			int curr_key = root->key[0];

			while (curr_key <= UpperBound)
			{
				if (cursor == nullptr)
				{
					break;
					// for empty b+ tree
				}
				else if (cursor->type == 1)
				{
				}
			}
		}

		int index = 0;
		// need to search for the lower bound first that
		// transfer the point to var start_node
	
		cursor = start_node;
		temp = cursor->item[0];

		while (temp <= end)
		{
			if (cursor == nullptr)
			{
				break;
				// for empty b+ tree
			}
			for (int i = 0; i < cursor->size; i++)
			{
				temp = cursor->item[i];
				if ((temp >= LowerBound) && (temp <= UpperBound))
				{
					// print the number and the content of index
					// print the number and the content of data blocks
					// print the average of "averageRating" of the records
					index++;
				}
			}
			cursor = cursor->children[cursor->size];
		}
		return index;
	}
};
