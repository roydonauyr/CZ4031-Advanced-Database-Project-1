#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>

#include "structures.hpp"

class BlockManager{
private:
    const int blkSize = 200;
    std::queue<unsigned int> deletedIndex;

    std::vector <block *> blockPtrArray;

public:
    unsigned int getSize() const;
    unsigned int getNumBlocks() const;
    void deleteBlock(unsigned int loc);
    //unsigned int createBlocks(unsigned int numBlocks);
    //unsigned int createBlock();
    unsigned int createRecordBlock();
    unsigned int createIndexBlock();
    unsigned int createRecordBlocks(unsigned int numBlocks);
    unsigned int createIndexBlocks(unsigned int numBlocks);
    block * accessBlock(unsigned int index) const;
};

unsigned int BlockManager::getSize() const {
    return blkSize * (blockPtrArray.size() - deletedIndex.size());
}

unsigned int BlockManager::getNumBlocks() const {
    return (blockPtrArray.size() - deletedIndex.size());
}

void BlockManager::deleteBlock(unsigned int loc){
   delete blockPtrArray[loc];
   deletedIndex.push(loc);
   blockPtrArray[loc] = NULL;
}

unsigned int BlockManager::createRecordBlock(){
    block * ptr = new RecordBlock;
    if(deletedIndex.empty()){
        blockPtrArray.push_back(ptr);
        return blockPtrArray.size() - 1;
    }
    else{
        unsigned int temp = deletedIndex.front();
        deletedIndex.pop();
        blockPtrArray[temp] = ptr;
        return temp;
    }
}

unsigned int BlockManager::createIndexBlock(){
    block * ptr = new treeNodeBlock;
    if(deletedIndex.empty()){
        blockPtrArray.push_back(ptr);
        return blockPtrArray.size() - 1;
    }
    else{
        unsigned int temp = deletedIndex.front();
        deletedIndex.pop();
        blockPtrArray[temp] = ptr;
        return temp;
    }
}

unsigned int BlockManager::createRecordBlocks(unsigned int numBlocks){
    unsigned int firstIndex = blockPtrArray.size();
    for (int i = 0; i < numBlocks; i++){
        block * ptr = new RecordBlock;
        blockPtrArray.push_back(ptr);
    }
    return firstIndex;
}

unsigned int BlockManager::createIndexBlocks(unsigned int numBlocks){
    unsigned int firstIndex = blockPtrArray.size();
    for (int i = 0; i < numBlocks; i++){
        block * ptr = new treeNodeBlock;
        blockPtrArray.push_back(ptr);
    }
    return firstIndex;
}

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

block * BlockManager::accessBlock(unsigned int index) const{
    return blockPtrArray[index];
}


int main(){

}