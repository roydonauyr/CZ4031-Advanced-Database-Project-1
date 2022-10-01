#ifndef STORAGE_HEADER
#define STORAGE_HEADER

#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <unordered_set>

#include "structures.hpp"

class BlockManager{
private:
    std::queue<unsigned int> deletedIndex;

    std::vector <block *> blockPtrArray;

public:
    const int blkSize = BLOCK_SIZE;
    const char recordsPerBlock = NUM_RECORDS;
    const char keyPerIndexBlock = NUM_KEY_INDEX;
    const char keyPerLinkedList = NUM_LINKED_LIST;
    unsigned long numStorageBlocks = 0;
    unsigned long numTreeBlocks = 0;
    unsigned int deletedNodes = 0;
    unsigned int getSize() const;
    unsigned int getNumBlocks() const;
    void deleteBlock(unsigned int loc);
    //unsigned int createBlocks(unsigned int numBlocks);
    //unsigned int createBlock();
    unsigned int createRecordBlock();
    unsigned int createIndexBlock();
    unsigned int createLinkedListBlock();
    unsigned int createRecordBlocks(unsigned int numBlocks);
    //unsigned int createIndexBlocks(unsigned int numBlocks);
    block * accessBlock(unsigned int index);
    block * noLogAccessBlock(unsigned int index) const;
    void clearAccessed();

    //Helper functions
    void printRecordBlock(unsigned int recordBlockIndex);

    std::unordered_set<unsigned int> accessedDataBlocks, accessedTreeBlocks;

    std::vector<unsigned int> firstData, firstTree;
};

//print records in recordBlock
void BlockManager::printRecordBlock(unsigned int recordBlockIndex){
    printf("\n------------------------------------------\n");
    RecordBlock* curNode = (RecordBlock*) accessBlock(recordBlockIndex);
    std::cout<<"Block id is: "<<recordBlockIndex<<std::endl;
    std::cout<<"Type is: "<<(unsigned int)curNode->type<<std::endl;
    std::cout<<"Records: "<<std::endl;
    for (Record record: curNode->records){
        record.printRecord();
    }
};

unsigned int BlockManager::getSize() const {
    return blkSize * (blockPtrArray.size() - deletedIndex.size());
}

unsigned int BlockManager::getNumBlocks() const {
    return (blockPtrArray.size() - deletedIndex.size());
}

void BlockManager::deleteBlock(unsigned int loc){
    loc--;
    if(blockPtrArray[loc]->type != 0){
        deletedNodes++;
        numTreeBlocks--;
    }
   delete blockPtrArray[loc];
   deletedIndex.push(loc);
   blockPtrArray[loc] = NULL;
}

unsigned int BlockManager::createRecordBlock(){
    numStorageBlocks++;
    block * ptr = new RecordBlock;
    ptr->type = 0;
    if(deletedIndex.empty()){
        blockPtrArray.push_back(ptr);
        return blockPtrArray.size() - 1 + 1;
    }
    else{
        unsigned int temp = deletedIndex.front();
        deletedIndex.pop();
        blockPtrArray[temp] = ptr;
        return temp;
    }
}

unsigned int BlockManager::createIndexBlock(){
    numTreeBlocks++;
    block * ptr = new treeNodeBlock;
    if(deletedIndex.empty()){
        blockPtrArray.push_back(ptr);
        return blockPtrArray.size() - 1 + 1;
    }
    else{
        unsigned int temp = deletedIndex.front();
        deletedIndex.pop();
        blockPtrArray[temp] = ptr;
        return temp;
    }
}

unsigned int BlockManager::createLinkedListBlock(){
    numTreeBlocks++;
    block * ptr = new linkedListNodeBlock;
    if(deletedIndex.empty()){
        blockPtrArray.push_back(ptr);
        return blockPtrArray.size() - 1 + 1;
    }
    else{
        unsigned int temp = deletedIndex.front();
        deletedIndex.pop();
        blockPtrArray[temp] = ptr;
        return temp;
    }
}

unsigned int BlockManager::createRecordBlocks(unsigned int numBlocks){
    numStorageBlocks+=numBlocks;
    unsigned int firstIndex = blockPtrArray.size();
    for (int i = 0; i < numBlocks; i++){
        block * ptr = new RecordBlock;
        ptr->type = 0;
        blockPtrArray.push_back(ptr);
    }
    return firstIndex + 1;
}

/*unsigned int BlockManager::createIndexBlocks(unsigned int numBlocks){
    unsigned int firstIndex = blockPtrArray.size();
    for (int i = 0; i < numBlocks; i++){
        block * ptr = new treeNodeBlock;
        blockPtrArray.push_back(ptr);
    }
    return firstIndex + 1;
}*/

/*unsigned int BlockManager::createBlock(){
    void* ptr = (void*) malloc(blkSize);
    if (deletedIndex.empty()){
        blockPtrArray.push_back(ptr);
        return blockPtrArray.size()-1;
    }else{
        unsigned int temp = deletedIndex.front();
        deletedIndex.pop();
        blockPtrArray[temp] = ptr;
        return temp;
    }
}

unsigned int BlockManager::createBlocks(unsigned int numBlocks){
    unsigned int firstIndex = blockPtrArray.size();
    for (int i = 0; i < numBlocks; i++){
        void* ptr = (void*) malloc(blkSize);
        blockPtrArray.push_back(ptr);
    }
    return firstIndex;
}*/

block * BlockManager::accessBlock(unsigned int index) {
    if(index > blockPtrArray.size()){
        return nullptr;
    }
    if(blockPtrArray[index-1]->type == 0){
        accessedDataBlocks.insert(index);
        if(firstData.size() < 5){
            if(std::find(firstData.begin(), firstData.end(), index) == firstData.end()){
                firstData.push_back(index);
            }
        }
    }
    else{
        accessedTreeBlocks.insert(index);
        if(firstTree.size() < 5){
            if(std::find(firstTree.begin(), firstTree.end(), index) == firstTree.end()){
                firstTree.push_back(index);
            }
        }
    }
    return blockPtrArray[index-1];
}

block * BlockManager::noLogAccessBlock(unsigned int index) const {
    return blockPtrArray[index-1];
}

void BlockManager::clearAccessed(){
    accessedDataBlocks.clear();
    accessedTreeBlocks.clear();
    firstTree.clear();
    firstData.clear();
}

#endif