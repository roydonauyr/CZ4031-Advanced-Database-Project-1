#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <cstdio>
#include <cmath>
#include "structures.hpp"
#include "BPlusTree.hpp"
#include "Database_Storage.hpp"
#include "utility.hpp"

using namespace std;

std::vector<Record> loadData(){
    std::ifstream dataFile;
    dataFile.open("data.tsv");
    dataFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::vector<Record> Records;
    while(dataFile.good()){
        Record toInsert;
        std::string tconst = "", avgRatingBuf = "";
        dataFile>>tconst;
        dataFile>>avgRatingBuf;
        dataFile>>toInsert.numVotes;
        toInsert.setTconst(tconst);
        const std::string ones = avgRatingBuf.substr(0, avgRatingBuf.find('.'));
        toInsert.avgRating.ones = std::stoi(ones);
        avgRatingBuf.erase(0, avgRatingBuf.find('.') + 1);
        const std::string decimal = avgRatingBuf;
        toInsert.avgRating.decimal = std::stoi(decimal);
        Records.push_back(toInsert);
        dataFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        dataFile.peek();
    }
    return Records;
}

// TODO
int main() {
    std::vector<Record> Records = loadData();


    BlockManager blkManager;
    BPlusTree tree = BPlusTree(&blkManager);
    const unsigned int numBlocksStore = (unsigned int) ceil((float)Records.size() / (float)blkManager.recordsPerBlock);
    const unsigned int firstBlock = blkManager.createRecordBlocks(numBlocksStore); // First block - firstBlock + numBlocksStore - 1
    unsigned int i = 0, curBlockIndex = firstBlock;
    RecordBlock *curBlock = (RecordBlock *) blkManager.noLogAccessBlock(curBlockIndex);
    while(i<Records.size()){
        //std::cout<<"INSERTING "<<Records[i].getTconst()<<" "<<Records[i].numVotes<<std::endl;
        curBlock->records[i%blkManager.recordsPerBlock] = Records[i];
        if((i%blkManager.recordsPerBlock) == (blkManager.recordsPerBlock-1)){
            curBlockIndex++;
            curBlock = (RecordBlock *) blkManager.accessBlock(curBlockIndex);
        }
        i++;
    }
    RecordBlock *test = (RecordBlock *)blkManager.accessBlock(firstBlock);
    Pointer ptr;
    ptr.setBlock(firstBlock);
    ptr.entry=0;
    i=0;
    curBlockIndex = firstBlock;
    curBlock= (RecordBlock *) blkManager.accessBlock(curBlockIndex);
    std::cout<<"------Exercise 1----------"<<std::endl;
    std::cout<<"Number of blocks used by storage is: "<<numBlocksStore<<std::endl;
    //std::cout<<"*TEST* Number of blocks used acc to blkManager"<<blkManager.numStorageBlocks<<std::endl;
    std::cout<<"Size used by storage is: "<<numBlocksStore * blkManager.blkSize<<"B"<<std::endl;
    tree.insert(test->records[0].numVotes, ptr);
    //Records.size()
    while(i<100000){

        ptr.setBlock(curBlockIndex);
        ptr.entry=i%blkManager.recordsPerBlock;
        //std::cout<<std::endl<<"INSERTING: "<<curBlock->records[i%blkManager.recordsPerBlock].numVotes<<" PTR: "<<ptr.getBlock()<<std::endl;
        tree.insert(curBlock->records[i%blkManager.recordsPerBlock].numVotes, ptr);
        if((i%blkManager.recordsPerBlock) == (blkManager.recordsPerBlock-1)){
            curBlockIndex++;
            curBlock = (RecordBlock *) blkManager.accessBlock(curBlockIndex);
        }
        i++;
    }
    // std::cout<<std::endl<<"------Exercise 2----------"<<std::endl;
    // //std::cout<<"Number of blocks used total is: "<<blkManager.getNumBlocks()<<std::endl;
    // std::cout<<"Number of nodes in B+ tree (Equal to number of blocks): "<<blkManager.numTreeBlocks<<std::endl;
    // std::cout<<"Height of B+ tree is: "<<getHeight(&tree)<<std::endl;
    // std::cout<<"Number of entries (n): "<<(int)blkManager.keyPerIndexBlock<<std::endl;
    // std::cout<<"Content of root node: "<<std::endl;
    // tree.printTreeNode(tree.rootNode);
    // std::cout<<"Content of 1st child node: "<<std::endl;
    // treeNodeBlock* root = (treeNodeBlock*)blkManager.accessBlock(tree.rootNode);
    // tree.printTreeNode(root->ptrs[0].getBlock());
    
    
    // std::cout<<std::endl<<"------Exercise 3----------"<<std::endl;
    // blkManager.clearAccessed();
    // std::vector<Record> recs = tree.searchKeys(500);
    // unsigned int totalSearchIO = blkManager.accessedTreeBlocks.size() + blkManager.accessedDataBlocks.size();
    // std::cout<<std::endl<<"Total Number of Index Nodes accessed (Unique): "<<blkManager.accessedTreeBlocks.size()<<std::endl;
    // std::cout<<std::endl<<"First 5 Unique Index Nodes accessed: "<<std::endl;
    // for(unsigned int nodeIndex: blkManager.firstTree){
    //     printf("%d||", nodeIndex);
    // }
    // for(unsigned int nodeIndex: blkManager.firstTree){
    //     if(blkManager.accessBlock(nodeIndex)->type == 3){
    //         tree.PrintLinkedListBlock(nodeIndex);
    //     }else{
    //         tree.printTreeNode(nodeIndex);
    //     }
    // }
    // std::cout<<std::endl<<"\n"<<"Total Number of Data Blocks accessed (Unique): "<<blkManager.accessedDataBlocks.size()<<std::endl;
    // std::cout<<std::endl<<"First 5 Unique Data Blocks accessed: "<<std::endl;
    // for(unsigned int recordBlockIndex: blkManager.firstData){
    //     printf("%d||", recordBlockIndex);
    // }
    // for(unsigned int recordBlockIndex: blkManager.firstData){
    //     blkManager.printRecordBlock(recordBlockIndex);
    // }
    // float averageOfAverageRating = 0.0;
    // for(Record record: recs){
    //     averageOfAverageRating += record.getAverageRating();
    // }
    // averageOfAverageRating = averageOfAverageRating/recs.size();
    // std::cout<<std::endl<<"\n"<<"Average of average ratings of all records: "<<averageOfAverageRating<<std::endl;
    // std::cout<<std::endl<<"Total SearchIO: "<<totalSearchIO<<std::endl;

    std::cout<<std::endl<<"------Exercise 4----------"<<std::endl;
    blkManager.clearAccessed();
    std::vector<Record> recs = tree.searchRangeOfKeys(30000,40000);
    unsigned int totalSearchIO = blkManager.accessedTreeBlocks.size() + blkManager.accessedDataBlocks.size();
    std::cout<<std::endl<<"Total Number of Index Nodes accessed (Unique): "<<blkManager.accessedTreeBlocks.size()<<std::endl;
    std::cout<<std::endl<<"First 5 Unique Index Nodes accessed: "<<std::endl;
    for(unsigned int nodeIndex: blkManager.firstTree){
        printf("%d||", nodeIndex);
    }
    for(unsigned int nodeIndex: blkManager.firstTree){
        if(blkManager.accessBlock(nodeIndex)->type == 3){
            tree.PrintLinkedListBlock(nodeIndex);
        }else{
            tree.printTreeNode(nodeIndex);
        }
    }
    std::cout<<std::endl<<"\n"<<"Total Number of Data Blocks accessed (Unique): "<<blkManager.accessedDataBlocks.size()<<std::endl;
    std::cout<<std::endl<<"First 5 Unique Data Blocks accessed: "<<std::endl;
    for(unsigned int recordBlockIndex: blkManager.firstData){
        printf("%d||", recordBlockIndex);
    }
    for(unsigned int recordBlockIndex: blkManager.firstData){
        blkManager.printRecordBlock(recordBlockIndex);
    }
    float averageOfAverageRating = 0.0;
    for(Record record: recs){
        averageOfAverageRating += record.getAverageRating();
    }
    averageOfAverageRating = averageOfAverageRating/recs.size();
    std::cout<<std::endl<<"\n"<<"Average of average ratings of all records: "<<averageOfAverageRating<<std::endl;
    std::cout<<std::endl<<"Total SearchIO: "<<totalSearchIO<<std::endl;
    
    std::cout<<"DONE"<<std::endl;
}