#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "structures.hpp"
#include "BPTree.cpp"
#include "Database_Storage.cpp"

using namespace std;

// TODO
int main() {
    std::ifstream dataFile;
    dataFile.open("data.tsv");
    dataFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    bool hasData=true;
    std::vector<Record> Records;
    while(dataFile.good()){
        Record toInsert;
        std::string tconst, avgRatingBuf, ones, decimal;
        dataFile>>tconst;
        dataFile>>avgRatingBuf;
        dataFile>>toInsert.numVotes;
        toInsert.setTconst(tconst);
        Records.push_back(toInsert);
        ones = avgRatingBuf.substr(0, avgRatingBuf.find('.'));
        toInsert.avgRating.ones = std::stoi(ones);
        avgRatingBuf.erase(0, avgRatingBuf.find('.') + 1);
        decimal = avgRatingBuf;
        toInsert.avgRating.decimal = std::stoi(decimal);
        dataFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        dataFile.peek();
    }
}