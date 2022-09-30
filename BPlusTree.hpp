#ifndef BPLUSTREE_HEADER
#define BPLUSTREE_HEADER

#include <algorithm>
#include <array>
#include <iterator>
#include <vector>
#include <utility>
#include <iostream>

#include "Database_Storage.hpp"

class BPlusTree
{
public:
	BlockManager * blkManager;
	
	unsigned int rootNode = 0; //Stores index of root node

	BPlusTree(BlockManager *mgr){
		this->blkManager = mgr;
	}

	// Print out a node block for BPlusTree
	void printTreeNode(unsigned int nodeIndex){
		treeNodeBlock* curNode = (treeNodeBlock*) blkManager->accessBlock(nodeIndex);
		printf("\n------------------------------------------\n");
		std::cout<<"Block id is: "<<nodeIndex<<std::endl;
		std::cout<<"Parent id is: "<<curNode->getParentBlock()<<std::endl;
		std::cout<<"Type is: "<<(unsigned int)curNode->type<<std::endl;
		std::cout<<"Keys: "<<std::endl;
		for (unsigned int key: curNode->key){
			std::cout<<key<<" ";
		}
		std::cout<<std::endl;
		std::cout<<"Ptrs: "<<std::endl;
		for (Pointer ptr: curNode->ptrs){
			std::cout<<ptr.getBlock()<<" ";
		}
		std::cout<<std::endl;
	}

	//Print linkedList Block
	void PrintLinkedListBlock(unsigned int LinkedListIndex){
		printf("\n------------------------------------------\n");
		linkedListNodeBlock* curNode = (linkedListNodeBlock*) blkManager->accessBlock(LinkedListIndex);
		std::cout<<"Block id is: "<<(unsigned int)LinkedListIndex<<std::endl;
		std::cout<<"Type is: "<<(unsigned int)curNode->type<<std::endl;
		std::cout<<"Record addresses: "<<std::endl;
		for (Pointer ptr: curNode->pointers){
			std::cout<<ptr.getBlock()<<" ";
		}
		std::cout<<"\n"<<"Next linkedList Block Pointer: "<<curNode->nextBlock.getBlock()<<std::endl;
	}

	//Search function
	std::vector<Record> searchKeys(unsigned int key){
		std::vector<Record> results;
		unsigned int curIndex = rootNode;
		treeNodeBlock* curBlock = (treeNodeBlock*) blkManager->accessBlock(rootNode);
		while(curBlock -> type != 2){
			unsigned int i = 0;
			while(i<curBlock->getLength() && key>curBlock->key[i]){
				i++;
			}
			curIndex = curBlock->ptrs[i].getBlock();
			curBlock = (treeNodeBlock*) blkManager->accessBlock(curIndex);
		}
		

		//Now curIndex and curBlock should be leaf node
		unsigned int j = 0;
		while(j<curBlock->getLength() && key!=curBlock->key[j]){
			j++;
		}
		if(j == curBlock->getLength()){
			std::cout<<"Key does not exist"<<std::endl;
			return results;
		}
		unsigned int curBlockIndex = curBlock->ptrs[j].getBlock();
		if (blkManager->accessBlock(curBlockIndex) ->type == 0){
			RecordBlock* curRecordBlock = (RecordBlock*)blkManager->accessBlock(curBlockIndex); 
			unsigned int accessIndex = curBlock->ptrs[j].entry;
			results.push_back(curRecordBlock->records[accessIndex]);
		}
		else if (blkManager -> accessBlock(curBlockIndex) ->type == 3){
			linkedListNodeBlock* linkedList = (linkedListNodeBlock*) blkManager -> accessBlock(curBlockIndex);
			//PrintLinkedListBlock(curBlockIndex);
			for(Pointer record_pointer: linkedList->pointers){
				if(record_pointer.getBlock() == 0){
					break;
				}
				unsigned int accessIndex = record_pointer.entry;
				unsigned int blockIndex = record_pointer.getBlock();
				RecordBlock* curRecordBlock = (RecordBlock*)blkManager->accessBlock(blockIndex);
				results.push_back(curRecordBlock->records[accessIndex]);
			}
			
			while(linkedList->nextBlock.getBlock() != 0){
				//Switch to next linkedlist
				unsigned int nextLinkedListIndex = linkedList->nextBlock.getBlock();
				linkedList = (linkedListNodeBlock*) blkManager -> accessBlock(nextLinkedListIndex);
				for(Pointer record_pointer: linkedList->pointers){
					if(record_pointer.getBlock() == 0){
						break;
					}
					unsigned int accessIndex = record_pointer.entry;
					unsigned int blockIndex = record_pointer.getBlock();
					RecordBlock* curRecordBlock = (RecordBlock*)blkManager->accessBlock(blockIndex);
					results.push_back(curRecordBlock->records[accessIndex]);
				}
			}
		}
		return results;
	}

	// std::vector<Record> searchKeys(unsigned int key)
	// {	
	// 	treeNodeBlock *root = (treeNodeBlock *)blkManager->accessBlock(rootNode); 
	// 	std::vector<Record> results;
	// 	std::vector<int> block_ids;
	// 	int id_counter = 0;

	// 	block_ids.push_back(rootNode);
	// 	id_counter++;
	
	// 	bool search_failed = false; // if no such key exists

	// 	block *cursor = root; // initialise cursor at root

	// 	while (cursor != nullptr || !search_failed) { // while loop till manually break or finish searching
	// 		switch (cursor->type) {
	// 			// for record blocks
	// 			case 0:
	// 			{
	// 				RecordBlock * record_block = (RecordBlock *) cursor; // casting

	// 				for (int i = 0; i < record_block->records.size(); i++) { // check all records for matching key
	// 					if (record_block->records[i].numVotes == key) {
	// 						results.push_back(record_block->records[i]); // push matching records into results set
	// 					} 
	// 				}

	// 				break;
	// 			}
				
	// 			//for interal nodes
	// 			case 1:
	// 			{
	// 				treeNodeBlock* internal_node = (treeNodeBlock *) cursor; // casting

	// 				for(int i = 0; i < internal_node->getLength(); i++) { // check all keys in index_node
						
	// 					if(internal_node->key[i] == key) { // if internal node key is equal to key, access block pointed by i+1 pointer
							
	// 						cursor = blkManager->accessBlock(internal_node->ptrs[i+1].getBlock()); // reset cursor to target block in next layer
							
	// 						if (id_counter++ < 5) {// print if total index prints less than 5
	// 							// do printing if printing works
	// 							block_ids.push_back(internal_node->ptrs[i+1].getBlock());
	// 						}

	// 						break;
	// 					}
	// 					else if(internal_node->key[i] > key) { // if internal node key is > key, access block pointed by i pointer
							
	// 						cursor = blkManager->accessBlock(internal_node->ptrs[i].getBlock()); // reset cursor to target block in next layer
							
	// 						if (id_counter++ < 5) {// print if total index prints less than 5
	// 							// do printing if printing works
	// 							block_ids.push_back(internal_node->ptrs[i+1].getBlock());
	// 						}

	// 						break;
	// 					}
	// 				}
	// 				break;
	// 			}
					

	// 			// for leaf nodes
	// 			// at the leaf node, can know if the search has failed or not
	// 			case 2:
	// 			{
	// 				treeNodeBlock * leaf_node = (treeNodeBlock *) cursor; 
	// 				search_failed = true;

	// 				for(int i = 0; i < leaf_node->getLength(); i++) { // check all leaf keys for matching
						
				// 		if (leaf_node->key[i] == key) { // if dense == key, if sparse >= key
				// 			cursor = blkManager->accessBlock(leaf_node->ptrs[i].getBlock()); // change cursor to the recordblock containing the matching record
				// 			search_failed = false;	// record has been found, split to duplicates vs nonduplicates
							
				// 			if (id_counter++ < 5) {// print if total index prints less than 5
				// 				// do printing if printing works
				// 				block_ids.push_back(leaf_node->ptrs[i+1].getBlock());
							
				// 			}

				// 			break; // stop searching as key is already found
				// 		}
				// 	}
				// 	break;
				// }

				// // for linkedlist block
				// case 3:
				// {
				// 	linkedListNodeBlock *linked_list_node_block = (linkedListNodeBlock *)cursor; 
				// 	RecordBlock* record_block_cursor; // cursor for parsing recordblocks
				// 	// NOTE please help to check protection on linkedlistnodeblock variables

				// 	for (int i = 0; i < linked_list_node_block->pointers.size(); i++) { // change function to pointer.size() when available, currently is boilerplate
				// 		record_block_cursor = (RecordBlock *)blkManager->accessBlock(linked_list_node_block->pointers[i].getBlock()); // update record block cursor
				// 		for (int i = 0; i < record_block_cursor->records.size(); i++) {// check all records in current recordblock
				// 		// check all records for matching key
						
				// 			if (record_block_cursor->records[i].numVotes == key) { // check if key matches could be removed leaving it in for sanity reasons
				// 				results.push_back(record_block_cursor->records[i]); // push matching records into results set
				// 			}
				// 		}
				// 	} 
		// 			if (linked_list_node_block->nextBlock.entry > -1) { // go to next linked block in linked list
		// 				cursor = blkManager->accessBlock(linked_list_node_block->nextBlock.getBlock()); // update cursor
		// 			}
		// 			break;
		// 		}
		// 	}
			
		// } return results; // return results set count be empty if not found ez
		
		// if (root == NULL)
		// {
		// 	cout << "Tree is empty\n";
		// }
		// else
		// {
		// 	Node *cursor = root;
		// 	// While loop until you find a leaf node
		// 	while (cursor->IS_LEAF == false)
		// 	{
		// 		// Apply binary search and once find the
		// 		for (int i = 0; i < cursor->size; i++)
		// 		{
		// 			if (key < cursor->item[i])
		// 			{
		// 				// find child node
		// 				cursor = cursor->children[i];
		// 				break;
		// 			}
		// 			if (i == (cursor->size) - 1)
		// 			{
		// 				cursor = cursor->children[i + 1];
		// 				break;
		// 			}
		// 		}
		// 	}
		// 	for (int i = 0; i < cursor->size; i++)
		// 	{
		// 		if (cursor->item[i] == key)
		// 		{
		// 			// Print out key details as well
		// 			// Check next children whether its the same key
		// 			// If key is the same need to print also
		// 			// Total five times if more than five
		// 			// Another While loop
		// 			return;
		// 		}
		// 	}
		// 	cout << "Not found\n";
		// 	// send not found not sure if need to return nullptr
		// }
	// }

	void deleteKey(unsigned int key, unsigned int curr, unsigned int child)
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
		if (key == 0) {
			std::cout << "Key is null." << std::endl;
			return;
		}

		treeNodeBlock *currNode = (treeNodeBlock*)(blkManager->accessBlock(curr));

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

				std::cout << "Root is too empty, changing root..." << std::endl;
				blkManager->deleteBlock(child);
			}
			else if (currNode->getLength() == 0) {
				std::cout << "No keys left in the tree. Killing tree..." << std::endl;
				rootNode = 0;
			}

			blkManager->deleteBlock(curr);
			return;
		}
		else {
			int leftSibling;			// Index of left sibling
			int rightSibling;			// Index of right sibling

			treeNodeBlock *cursor = (treeNodeBlock*)(blkManager->accessBlock(rootNode));
			// Loop through entire B+ tree until a leaf node is reached
			while (cursor->type != 2) {
				int index = 0;
				// Find the node that might contain the key
				for (int i = 0; i < cursor->getLength(); i++) {
					if (key < cursor->key[i] || i == cursor->getLength()) {
						// Go to child node
						cursor = (treeNodeBlock*)(blkManager->accessBlock(cursor->ptrs[i].getBlock()));
						if (currNode == NULL) {
							currNode = (treeNodeBlock*)(cursor);
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
					std::cout << "Key found, deleting key..." << std::endl;
				}
			}

			// Key not in B+ tree
			if (!isDeleted) {
				std::cout << "Key not found." << std::endl;
				return;
			}

			// Delete linked list
			if (blkManager->accessBlock(currNode->ptrs[index].getBlock())->type == 2) {
				linkedListNodeBlock *toBeDeleted = static_cast<linkedListNodeBlock*>(blkManager->accessBlock(currNode->ptrs[index].getBlock()));

				while (toBeDeleted->nextBlock.entry != -1) {
					linkedListNodeBlock *currDelete = toBeDeleted;
					toBeDeleted = static_cast<linkedListNodeBlock*>(blkManager->accessBlock(toBeDeleted->nextBlock.getBlock()));
					blkManager->deleteBlock(currDelete->nextBlock.getBlock());
				}
			}
			else if (blkManager->accessBlock(currNode->ptrs[index].getBlock())->type == 0) {
				RecordBlock *toBeDeleted = static_cast<RecordBlock*>(blkManager->accessBlock(currNode->ptrs[index].getBlock()));
				blkManager->deleteBlock(currNode->ptrs[index].getBlock());
			}

			// Delete key
			currNode->key[index] = 0;
			currNode->ptrs[index].entry = 0;

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
				leftNode = (treeNodeBlock*)blkManager->accessBlock(parent->ptrs[leftSibling].getBlock());
			}

			// Check if right sibling exists
			if (rightSibling <= blkManager->keyPerIndexBlock + 1) {
				rightNode = (treeNodeBlock*)blkManager->accessBlock(parent->ptrs[rightSibling].getBlock());
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
			else if (rightNode != NULL && rightNode->getLength() >= ceil((blkManager->keyPerIndexBlock + 1) / 2)) {			
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
		std::cout << "Deleting node..." << std::endl;
		blkManager->deleteBlock(curr);
		return;
	}

	// Assuming that we have a root node, finds the block which should contain a particular key.
	// Block need not have space to contain this key
	unsigned int searchBlockToContain(unsigned int key){
		unsigned int curIndex = rootNode;
		treeNodeBlock* curBlock = (treeNodeBlock*) blkManager->accessBlock(rootNode);
		if(curBlock -> type == 2){
			return curIndex;
		}
		while(curBlock -> type != 2){
			unsigned int i = 0;
			// C 1 A 4 B
			while(i<curBlock->getLength() && key>curBlock->key[i]){
				i++;
			}
			curIndex = curBlock->ptrs[i].getBlock();
			curBlock = (treeNodeBlock*) blkManager->accessBlock(curIndex);
		}
		return curIndex;
	}

	void insertKeyToLinkedList(Pointer ptr, linkedListNodeBlock* firstLinkedListNode){
		linkedListNodeBlock* curLinkedList = firstLinkedListNode;
		// Linked list block exist, insert new entry
		unsigned int i = 0;
		while(curLinkedList->pointers[i].getBlock() != 0){
			//This pointer points to a block already
			if(i==(curLinkedList->pointers.size()-1)){
				// This block is full
				if(curLinkedList->nextBlock.getBlock() == 0){
					// No next block, so need to create new block and add into it
					unsigned int newLinkedListIndex = blkManager->createLinkedListBlock();
					linkedListNodeBlock* newLinkedList = (linkedListNodeBlock*)blkManager->accessBlock(newLinkedListIndex);
					newLinkedList->type = 3;
					newLinkedList->pointers[0];
					Pointer newPtr;
					newPtr.setBlock(newLinkedListIndex);
					curLinkedList->nextBlock = newPtr;
					newLinkedList->pointers[0] = ptr;
					return;
				}
				else{
					// Continue searching next block
					i = 0;
					curLinkedList = (linkedListNodeBlock *)blkManager->accessBlock(curLinkedList->nextBlock.getBlock());
				}
			}
			else{
				i++;
			}
		}
		curLinkedList->pointers[i] = ptr;
	}
	//Find lowest bound of a treeNodeBlock
	unsigned int lowestBound(treeNodeBlock* block) const {
		treeNodeBlock* curBlock = block;
		while(curBlock->type != 2){
			curBlock = (treeNodeBlock *)blkManager->accessBlock(curBlock->ptrs[0].getBlock());
		}
		return curBlock->key[0];
	}

	void insertInternal(unsigned int curBlockIndex , const Pointer ptr){
		//printf("---------INSERT INTERNAL----------\n");
		treeNodeBlock* curBlock = (treeNodeBlock*)blkManager->accessBlock(curBlockIndex);
		//unsigned int key = lowestBound((treeNodeBlock*)blkManager->accessBlock(ptr.getBlock()));
		if(curBlock->getLength() < curBlock->key.size()){
			// Has space, just insert
			/*unsigned int locationToInsert = 0;
			while(curBlock->key[locationToInsert] < key && locationToInsert < curBlock->getLength()){
				locationToInsert++;
			}
			if(locationToInsert == (curBlock->key.size() - 1) ){
				curBlock->key[locationToInsert] = key;
				curBlock->ptrs[locationToInsert+ 1] = ptr;
			}
			else{
				unsigned int i = 0;
				std::vector<Pointer> ptrTemp;
				std::vector<unsigned int> keyTemp;
				while(i<curBlock->key.size()){
					i++;
				}
				i=0;
				while(i<curBlock->ptrs.size()){
					i++;
				}
				
				
				//std::move_backward(curBlock->ptrs.begin() + locationToInsert + 1, curBlock->ptrs.end() - 1, curBlock->ptrs.end());
				//std::move_backward(curBlock->key.begin() + locationToInsert, curBlock->key.end() - 1, curBlock->key.end());
				//curBlock->ptrs[locationToInsert+ 1] = ptr;
				//curBlock->key[locationToInsert] = key;
			}*/

			std::vector<Pointer> ptrTemp;
			unsigned int i=0;
			while(i<(curBlock->ptrs.size()-1) && curBlock->ptrs[i].getBlock() != 0){
				ptrTemp.push_back(curBlock->ptrs[i]);
				i++;
			}
			ptrTemp.push_back(ptr);
			std::sort(ptrTemp.begin(), ptrTemp.end(), [&](Pointer a, Pointer b){
				unsigned int aKey = lowestBound((treeNodeBlock*)blkManager->accessBlock(a.getBlock()));
				unsigned int bKey = lowestBound((treeNodeBlock*)blkManager->accessBlock(b.getBlock()));
				return aKey < bKey;
			});
			curBlock->ptrs[0] = ptrTemp[0];
			i=1;
			while(i<ptrTemp.size()){
				curBlock->ptrs[i] = ptrTemp[i];
				curBlock->key[i-1] = lowestBound((treeNodeBlock*)blkManager->accessBlock(ptrTemp[i].getBlock()));
				i++;
			}

			treeNodeBlock* childBlock = (treeNodeBlock*)blkManager->accessBlock(ptr.getBlock());
			childBlock->setParentBlock(curBlockIndex);
			//printTreeNode(curBlockIndex);
		}
		else{
			// No space, need to split
			std::vector<Pointer> pointers;
			for(Pointer p: curBlock->ptrs){
				pointers.push_back(p);
			}
			unsigned int i = 0;
			while(i<curBlock->key.size()){
				curBlock->ptrs[i].setBlock(0);
				curBlock->ptrs[i].entry=0;
				curBlock->key[i] = 0;
				i++;
			}
			curBlock->ptrs[curBlock->ptrs.size()-1].setBlock(0);
			curBlock->ptrs[curBlock->ptrs.size()-1].entry=0;
			std::sort(pointers.begin(), pointers.end(), [&](Pointer a, Pointer b){
				unsigned int aKey = lowestBound((treeNodeBlock*)blkManager->accessBlock(a.getBlock()));
				unsigned int bKey = lowestBound((treeNodeBlock*)blkManager->accessBlock(b.getBlock()));
				return aKey < bKey;
			});
			unsigned int ptrInNewNode = curBlock->key.size() / 2;
			unsigned int ptrInCurNode = pointers.size() - ptrInNewNode;

			unsigned int newTreeNodeIndex = blkManager->createIndexBlock();
			treeNodeBlock* newTreeNode = (treeNodeBlock*)blkManager->accessBlock(newTreeNodeIndex);
			newTreeNode->type = 1;
			curBlock->ptrs[0] = pointers[0];

			i = 1;
			while(i<ptrInCurNode){
				curBlock->ptrs[i] = pointers[i];
				curBlock->key[i-1] = lowestBound((treeNodeBlock*)blkManager->accessBlock(pointers[i].getBlock()));
				i++;
			}
			newTreeNode->ptrs[0] = pointers[i];
			i++;
			unsigned int j=1;
			while(j<ptrInNewNode){
				newTreeNode->ptrs[j] = pointers[i];
				newTreeNode->key[j-1] = lowestBound((treeNodeBlock*)blkManager->accessBlock(pointers[i].getBlock()));
				j++;
				i++;
			}
			i = 0;
			while(i<ptrInCurNode){
				treeNodeBlock* updateParent = (treeNodeBlock*)blkManager->accessBlock(curBlock->ptrs[i].getBlock());
				updateParent->setParentBlock(curBlockIndex);
				i++;
			}
			i=0;
			while(i<ptrInNewNode){
				treeNodeBlock* updateParent = (treeNodeBlock*)blkManager->accessBlock(newTreeNode->ptrs[i].getBlock());
				updateParent->setParentBlock(newTreeNodeIndex);
				i++;
			}
			if(curBlockIndex == rootNode){
				// is root, make new root
				unsigned int newRootIndex = blkManager->createIndexBlock();
				treeNodeBlock* newRoot = (treeNodeBlock*)blkManager->accessBlock(newRootIndex);
				newRoot->type = 1;
				rootNode = newRootIndex;

				Pointer firstPtr, secondPtr;
				firstPtr.setBlock(curBlockIndex);
				secondPtr.setBlock(newTreeNodeIndex);
				newRoot->ptrs[0] = firstPtr;
				newRoot->ptrs[1] = secondPtr;
				newRoot->key[0] = lowestBound(newTreeNode);

				// Update parents of 2 blocks
				curBlock->setParentBlock(newRootIndex);
				newTreeNode->setParentBlock(newRootIndex);
			}
			else{
				// Update parent
				Pointer toAddPtr;
				toAddPtr.setBlock(newTreeNodeIndex);
				insertInternal(curBlock->getParentBlock(), toAddPtr);
			}
			//printTreeNode(curBlockIndex);
			//printTreeNode(newTreeNodeIndex);

		}
	}

	void insert(unsigned int key, Pointer ptr){
		//printf("----------INSERT-----------\n");
		if(rootNode == 0){
			//Empty tree, create new tree
			rootNode = blkManager->createIndexBlock();
			treeNodeBlock* rootBlock = (treeNodeBlock*)blkManager->accessBlock(rootNode);
			rootBlock->type = 2;
			rootBlock->key[0] = key;
			rootBlock->ptrs[0] = ptr;
			//printTreeNode(rootNode);
		}
		else{
			const unsigned int blockToInsert = searchBlockToContain(key);
			//printf("BLOCK TO INSERT: %d\n", blockToInsert);
			treeNodeBlock* insertBlock = (treeNodeBlock*)blkManager->accessBlock(blockToInsert);
			//if(insertBlock->getLength() < insertBlock->key.size()){
			if(insertBlock->getLength() < insertBlock->key.size()){
				// Have space, insert into block and done
				unsigned int locationToInsert = 0;
				while(insertBlock->key[locationToInsert] < key && locationToInsert < insertBlock->getLength()){
					locationToInsert++;
				}
				if(insertBlock->key[locationToInsert] == key){
					// Duplicate found
					const Pointer pointed = insertBlock->ptrs[locationToInsert];
					block * pointedBlock = blkManager->accessBlock(pointed.getBlock());
					if(pointedBlock->type == 3){
						// Linked list block exist, insert new entry
						insertKeyToLinkedList(ptr, (linkedListNodeBlock*)pointedBlock);
					}
					else{
						// No linked list block, create new linked list block
						unsigned int newLinkedListIndex = blkManager->createLinkedListBlock();
						linkedListNodeBlock* newLinkedList = (linkedListNodeBlock*)blkManager->accessBlock(newLinkedListIndex);
						newLinkedList->type = 3;
						newLinkedList->pointers[0] = pointed;
						newLinkedList->pointers[1] = ptr;
						Pointer newLinkedListPointer;
						newLinkedListPointer.setBlock(newLinkedListIndex);
						insertBlock->ptrs[locationToInsert] = newLinkedListPointer;
					}
				}
				else{
					// No duplicate, shift all other keys and pointers back
					// Move Nth - getLength()th key and ptr back by 1
					// Insert to Nth index
					//const Pointer IAmNotArsedToDoAWhileLoop = insertBlock->ptrs[insertBlock->ptrs.size() -1 ];
					
					std::vector<unsigned int> keyTemp;
					std::vector<Pointer> ptrTemp;

					unsigned int i=0;
					while(i<(insertBlock->key.size()-1)){
						keyTemp.push_back(insertBlock->key[i]);
						ptrTemp.push_back(insertBlock->ptrs[i]);
						i++;
					}
					ptrTemp.insert(ptrTemp.begin() + locationToInsert, ptr );
					keyTemp.insert(keyTemp.begin() + locationToInsert, key );
					//std::move_backward(insertBlock->ptrs.begin() + locationToInsert, insertBlock->ptrs.end() - 1, insertBlock->ptrs.end());
					//std::copy(insertBlock->ptrs.begin() + locationToInsert, insertBlock->ptrs.end(), std::back_inserter(ptrTemp));
					
					//std::copy(insertBlock->key.begin() + locationToInsert, insertBlock->key.end(), std::back_inserter(keyTemp));
					//std::move_backward(insertBlock->key.begin() + locationToInsert, insertBlock->key.end() - 1, insertBlock->key.end());
					i=0;
					while(i<keyTemp.size()){
						//std::cout<<"Temp Key: "<<keyTemp[i]<<" ptr: "<<ptrTemp[i].getBlock();
						//insertBlock->ptrs[i].setBlock(ptrTemp[i].getBlock());
						insertBlock->ptrs[i] = ptrTemp[i];
						insertBlock->key[i] = keyTemp[i];
						i++;
					}
					insertBlock->ptrs[locationToInsert] = ptr;
					insertBlock->key[locationToInsert] = key;
					//std::cout<<std::endl;
					//std::copy(ptrTemp.begin(), ptrTemp.end(), insertBlock->ptrs.begin() + locationToInsert);
					//std::copy(keyTemp.begin(), keyTemp.end(), insertBlock->key.begin() + locationToInsert);

					//insertBlock->ptrs[insertBlock->ptrs.size() -1 ] = IAmNotArsedToDoAWhileLoop;
					//printTreeNode(blockToInsert);
					
				}
			}
			else{
				// No space, need to check if duplicate exists
				unsigned int locationToInsert = 0;
				while(insertBlock->key[locationToInsert] < key && locationToInsert < insertBlock->getLength()){
					locationToInsert++;
				}
				if(insertBlock->key[locationToInsert] == key){
					// Duplicate found
					Pointer pointed = insertBlock->ptrs[locationToInsert];
					block * pointedBlock = blkManager->accessBlock(pointed.getBlock());
					if(pointedBlock->type == 3){
						// Linked list block exist, insert new entry
						insertKeyToLinkedList(ptr, (linkedListNodeBlock*)pointedBlock);
					}
					else{
						// No linked list block, create new linked list block
						unsigned int newLinkedListIndex = blkManager->createLinkedListBlock();
						linkedListNodeBlock* newLinkedList = (linkedListNodeBlock*)blkManager->accessBlock(newLinkedListIndex);
						newLinkedList->type = 3;
						newLinkedList->pointers[0] = pointed;
						newLinkedList->pointers[1] = ptr;
						Pointer newLinkedListPointer;
						newLinkedListPointer.setBlock(newLinkedListIndex);
						insertBlock->ptrs[locationToInsert] = newLinkedListPointer;
					}
					//printTreeNode(blockToInsert);
				}
				else{
					// No duplicates exists, need to split
					// Gather all keys and ptr
					std::vector<std::pair<Pointer, unsigned int>> pointers;
					unsigned int i = 0;
					while(i<insertBlock->key.size()){
						pointers.push_back(std::make_pair(insertBlock->ptrs[i], insertBlock->key[i]));
						i++;
					}
					pointers.push_back(std::make_pair(ptr, key));
					i=0;
					while(i<insertBlock->key.size()){
						insertBlock->ptrs[i].setBlock(0);
						insertBlock->ptrs[i].entry=0;
						insertBlock->key[i]=0;
						i++;
					}
					std::sort(pointers.begin(), pointers.end(), [](std::pair<Pointer, unsigned int> a, std::pair<Pointer, unsigned int> b){
						return a.second < b.second;
					});
					const unsigned int recordInNewBlock = (insertBlock->key.size() + 1) / 2;
					const unsigned int recordInInsertBlock = pointers.size() - recordInNewBlock;
					i=0;
					// Fill up old block
					while(i<recordInInsertBlock){
						insertBlock->ptrs[i] = pointers[i].first;
						insertBlock->key[i] = pointers[i].second;
						i++;
					}

					// Create new block and fill up
					const unsigned int newTreeBlockIndex = blkManager->createIndexBlock();
					treeNodeBlock *newTreeBlock = (treeNodeBlock *)blkManager->accessBlock(newTreeBlockIndex);
					newTreeBlock->type = 2;
					unsigned int j=0;
					while(i<pointers.size()){
						newTreeBlock->ptrs[j] = pointers[i].first;
						newTreeBlock->key[j] = pointers[i].second;
						j++;
						i++;
					}
					// Copy over what the current node points to as next block to the new block
					newTreeBlock->ptrs[newTreeBlock->ptrs.size()-1] = insertBlock->ptrs[insertBlock->ptrs.size()-1];

					// Set next block for current block to be new block
					insertBlock->ptrs[insertBlock->ptrs.size()-1].setBlock(newTreeBlockIndex);

					
					if(blockToInsert == rootNode){
						unsigned int newParentIndex = blkManager->createIndexBlock();

						//Set parent block index
						newTreeBlock->setParentBlock(newParentIndex);
						insertBlock->setParentBlock(newParentIndex);
						treeNodeBlock *newParent = (treeNodeBlock *)blkManager->accessBlock(newParentIndex);
						newParent->type = 1;
						// Root node is current only leaf node. The new parent will be the root.
						rootNode = newParentIndex;

						newParent->ptrs[0].setBlock(blockToInsert);
						newParent->ptrs[1].setBlock(newTreeBlockIndex);

						newParent->key[0] = newTreeBlock->key[0];
					}
					
					else{
						Pointer toAddPtr;
						toAddPtr.setBlock(newTreeBlockIndex);
						insertInternal(insertBlock->getParentBlock(), toAddPtr);
					}
					//printTreeNode(blockToInsert);
					//printTreeNode(newTreeBlockIndex);
				}
			}
		}
	}
};

#endif