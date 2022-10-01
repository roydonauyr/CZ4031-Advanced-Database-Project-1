#ifndef STRUCTURES_HEADER
#define STRUCTURES_HEADER

#include <algorithm>
#include <array>
#include <string>
#include <cstring>

#define BLOCK_SIZE 500//200//500
#define NUM_RECORDS (BLOCK_SIZE-1)/16
#define NUM_KEY_INDEX (BLOCK_SIZE-4-4)/8
#define NUM_LINKED_LIST (BLOCK_SIZE-1-4)/4

#pragma pack(1)
// Ugh so to explain, pragma pack forces the compiler to not add padding.
// GENERALLY A BAD IDEA

// Fixed point x.x format, each using 1 byte which is more than enough
struct fixedPoint{
    unsigned char ones = 0, decimal = 0;
};

// 16 bytes Record
struct Record{
public:
    fixedPoint avgRating;
    unsigned int numVotes = 0;
    std::string getTconst() const {
        char buf[11];
        strncpy(buf, this->tconst, 10);
        buf[10] = '\0';
        return std::string(buf);
    }

    void setTconst(std::string input){
        memset(tconst,0,10);
        strncpy(tconst,input.c_str(),10);
    }

    void printRecord() const {
        std::cout<<"----Record details----"<<std::endl;

        printf("Average rating is: %.1f\n",getAverageRating());
        std::cout<<"Num votes is: "<<numVotes<<std::endl;
        std::cout<<"Tconst is: "<<getTconst()<<"\n"<<std::endl;
        
    }

    //To be used for calculation
    float getAverageRating() const {
        return float(avgRating.ones) + float(avgRating.decimal)/10;
    }

private:
    char tconst[10];
};

// Super class of blocks to check type of blocks to cast accordingly.
class block{
public:
    unsigned char type; // Check this value before casting your block into the derived classes.
    // 0: recordBlock, 1: tree non-leaf, 2: tree leaf, 3: linked list pointer block
};

// TODO: Change array sizes based on dynamic calculation
// 12 for 200B blocks because floor(200/16) = 12, with 8 bytes left over for other purposes.
struct RecordBlock: public block{
    std::array<Record, NUM_RECORDS> records; //Can use records.size()
    
    char padding[BLOCK_SIZE-(NUM_RECORDS*16+1)]; //Currently there for padding purposes, we can use this for something else later

};

//4 Bytes to save space. Oh no help me the bytewise manipulations
class Pointer{
public:
    char entry = -1; // Points to exact record, for dense index leaf nodes
    // We can use a negative value to indicate it does not point to data. Hence we know that it points to a tree

    unsigned long getBlock() const {
        return (block[2] << 16) + (block[1] << 8) + block[0];
    }

    void setBlock(unsigned long blockNum){
        block[2] = (blockNum & 0xFF0000) >> 16;
        block[1] = (blockNum & 0xFF00) >> 8;
        block[0] = blockNum & 0xFF;
    }

private:
    std::array<unsigned char, 3> block = { }; // Points to block. 0 should be a special value.
    // Block is accessed as MSB [2],[1],[0] LSB
    // This is private, we will use getters and setters here.
};

// 200B block
class treeNodeBlock: public block{
public:
    std::array<unsigned int, NUM_KEY_INDEX> key = { 0 }; // Minimum is 5, so we make 0 a special value.
    std::array<Pointer, NUM_KEY_INDEX + 1> ptrs;

    // Returns number of keys
    unsigned int getLength(){
        unsigned int i=0, count=0;
        while(i<key.size()){
            if(key[i] == 0){
                count++;
            }
            i++;
        }
        return key.size()-count;
    }

    unsigned long getParentBlock() const {
        return (parentBlock[2] << 16) + (parentBlock[1] << 8) + parentBlock[0];
    }

    void setParentBlock(unsigned long blockNum){
        parentBlock[2] = (blockNum & 0xFF0000) >> 16;
        parentBlock[1] = (blockNum & 0xFF00) >> 8;
        parentBlock[0] = blockNum & 0xFF;
    }
private:
    unsigned char parentBlock[3];
};

class linkedListNodeBlock: public block{
public:
    char padding[BLOCK_SIZE - 4 - 1 - NUM_LINKED_LIST * 4];
    std::array<Pointer, NUM_LINKED_LIST> pointers;
    Pointer nextBlock;
};

#endif