#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>

class BlockManager{
private:
    const int blkSize = 200;
    std::queue<unsigned int> deletedIndex;

    std::vector <void *> blockPtrArray;

public:
    unsigned int getSize();
    unsigned int getNumBlocks();
    void deleteBlock(unsigned int loc);
    unsigned int createBlocks(unsigned int numBlocks);
    unsigned int createBlock();
    void * accessBlock(unsigned int index);
};

unsigned int BlockManager::getSize(){
    return blkSize * blockPtrArray.size();
}

unsigned int BlockManager::getNumBlocks(){
    return blockPtrArray.size();
}

void BlockManager::deleteBlock(unsigned int loc){
   free(blockPtrArray[loc]);
   deletedIndex.push(loc);
   blockPtrArray[loc] = 0;
}

unsigned int BlockManager::createBlock(){
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
}

void * BlockManager::accessBlock(unsigned int index){
    return blockPtrArray[index];
}


int main(){

}