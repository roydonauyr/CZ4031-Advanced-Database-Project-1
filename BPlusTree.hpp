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
			while(i<curBlock->getLength() && key<curBlock->key[i]){
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
		treeNodeBlock* curBlock = (treeNodeBlock*)blkManager->accessBlock(curBlockIndex);
		unsigned int key = lowestBound((treeNodeBlock*)blkManager->accessBlock(ptr.getBlock()));
		if(curBlock->getLength() < curBlock->key.size()){
			// Has space, just insert
			unsigned int locationToInsert = 0;
			while(curBlock->key[locationToInsert] < key && locationToInsert < curBlock->getLength()){
				locationToInsert++;
			}
			if(locationToInsert == (curBlock->key.size() - 1) ){
				curBlock->key[locationToInsert] = key;
				curBlock->ptrs[locationToInsert+ 1] = ptr;
			}
			else{
				std::move_backward(curBlock->ptrs.begin() + locationToInsert + 1, curBlock->ptrs.end() - 1, curBlock->ptrs.end());
				std::move_backward(curBlock->key.begin() + locationToInsert, curBlock->key.end() - 1, curBlock->key.end());
				curBlock->ptrs[locationToInsert+ 1] = ptr;
				curBlock->key[locationToInsert] = key;
			}
			treeNodeBlock* childBlock = (treeNodeBlock*)blkManager->accessBlock(ptr.getBlock());
			childBlock->setParentBlock(curBlockIndex);
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
				curBlock->ptrs[j] = pointers[i];
				curBlock->key[j-1] = lowestBound((treeNodeBlock*)blkManager->accessBlock(pointers[i].getBlock()));
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
		}
	}

	void insert(unsigned int key, Pointer ptr){
		if(rootNode == 0){
			//Empty tree, create new tree
			rootNode = blkManager->createIndexBlock();
			treeNodeBlock* rootBlock = (treeNodeBlock*)blkManager->accessBlock(rootNode);
			rootBlock->type = 2;
			rootBlock->key[0] = key;
			rootBlock->ptrs[0] = ptr;
		}
		else{
			const unsigned int blockToInsert = searchBlockToContain(key);
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
					}
				}
				else{
					// No duplicate, shift all other keys and pointers back
					// Move Nth - getLength()th key and ptr back by 1
					// Insert to Nth index
					Pointer IAmNotArsedToDoAWhileLoop = insertBlock->ptrs[insertBlock->ptrs.size() -1 ];

					std::move_backward(insertBlock->ptrs.begin() + locationToInsert, insertBlock->ptrs.end() - 1, insertBlock->ptrs.end());
					insertBlock->ptrs[insertBlock->ptrs.size() -1 ] = IAmNotArsedToDoAWhileLoop;

					std::move_backward(insertBlock->key.begin() + locationToInsert, insertBlock->key.end() - 1, insertBlock->key.end());
					insertBlock->ptrs[locationToInsert] = ptr;
					insertBlock->key[locationToInsert] = key;
				}
			}
			else{
				std::cout<<"TEST"<<std::endl;
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
					}
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
						insertBlock->ptrs[j] = pointers[i].first;
						insertBlock->key[j] = pointers[i].second;
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
				}
			}
		}
	}
};