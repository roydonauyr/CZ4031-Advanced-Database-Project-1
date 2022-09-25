#ifndef STRUCTURES_HEADER
#define STRUCTURES_HEADER

#include <algorithm>
#include <array>
#include <string>

#pragma pack(1)
// Ugh so to explain, pragma pack forces the compiler to not add padding.
// GENERALLY A BAD IDEA

// Fixed point x.x format, each using 1 byte which is more than enough
struct fixedPoint{
    unsigned char ones, decimal;
};

// 16 bytes Record
struct Record{
public:
    fixedPoint avgRating;
    unsigned int numVotes;
    std::string getTconst(){
        char buf[11];
        strncpy(buf, this->tconst, 10);
        buf[11] = '\0';
        return std::string(buf);
    }

    void setTconst(std::string input){
        memset(tconst,0,10);
        strncpy(tconst,input.c_str(),10);
    }

private:
    char tconst[10];
};

// Super class of blocks to check type of blocks to cast accordingly.
class block{
public:
    unsigned char type; // Check this value before casting your block into the derived classes.
    // 0: recordBlock, 1: tree non-leaf, 2: tree leaf
};

// TODO: Change array sizes based on dynamic calculation
// 12 for 200B blocks because floor(200/16) = 12, with 8 bytes left over for other purposes.
struct RecordBlock: public block{
    Record records[12];
    
    char padding[7]; //Currently there for padding purposes, we can use this for something else later

    static RecordBlock* voidToRecord(void* blockPtr){
        return (RecordBlock*)(blockPtr); //UNSAFE
    }

    unsigned char getType(){
        return this->type;
    }
};

//4 Bytes to save space. Oh no help me the bytewise manipulations
class Pointer{
public:
    char entry; // Points to exact record, for dense index leaf nodes
    // We can use a negative value to indicate it does not point to data. Hence we know that it points to a tree

    unsigned long getBlock(){
        return (block[2] << 16) + (block[1] << 8) + block[0];
    }

    void setBlock(unsigned long blockNum){
        block[2] = blockNum & 0xFF0000;
        block[1] = blockNum & 0xFF00;
        block[0] = blockNum & 0xFF;
    }

private:
    unsigned char block[3]; // Points to block. 0 should be a special value.
    // Block is accessed as MSB [2],[1],[0] LSB
    // This is private, we will use getters and setters here.
};

// 200B block
class treeNodeBlock: public block{
public:
    std::array<unsigned int, 24> key = { }; // Minimum is 5, so we make 0 a special value.
    Pointer ptrs[25];
    unsigned int getLength(){
        return key.size() - std::count(key.cbegin(), key.cend(), 0);
    }

    unsigned long getParentBlock(){
        return (parentBlock[2] << 16) + (parentBlock[1] << 8) + parentBlock[0];
    }

    void setParentBlock(unsigned long blockNum){
        parentBlock[2] = blockNum & 0xFF0000;
        parentBlock[1] = blockNum & 0xFF00;
        parentBlock[0] = blockNum & 0xFF;
    }
private:
    unsigned char parentBlock[3];
};

#endif