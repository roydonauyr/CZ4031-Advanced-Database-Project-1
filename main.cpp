#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <cstdio>
#include <cmath>
#include "structures.hpp"
//#include "BPTree.cpp"
#include "Database_Storage.hpp"

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
    const unsigned int numBlocksStore = (unsigned int) ceil((float)Records.size() / (float)blkManager.recordsPerBlock);
    const unsigned int firstBlock = blkManager.createRecordBlocks(numBlocksStore);
    unsigned int i = 0, curBlockIndex = firstBlock;
    RecordBlock *curBlock = (RecordBlock *) blkManager.accessBlock(curBlockIndex);
    while(i<Records.size()){
        curBlock->records[i%blkManager.recordsPerBlock] = Records[i];
        if((i%blkManager.recordsPerBlock) == (blkManager.recordsPerBlock-1)){
            curBlockIndex++;
            curBlock = (RecordBlock *) blkManager.accessBlock(curBlockIndex);
        }
        i++;
    }

    std::cout<<"Number of blocks used by storage is: "<<numBlocksStore<<std::endl;
    std::cout<<"Size used by storage is: "<<numBlocksStore * blkManager.blkSize<<"B"<<std::endl;
}