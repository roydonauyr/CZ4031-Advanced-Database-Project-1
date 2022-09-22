// Fixed point x.x format, each using 1 byte which is more than enough
struct fixedPoint{
    unsigned char ones, decimal;
};

// 16 bytes Record
struct Record{
public:
    char tconst[10];
    fixedPoint avgRating;
    unsigned int numVotes;
};

// TODO: Change array sizes based on dynamic calculation
// 12 for 200B blocks because floor(200/16) = 12, with 8 bytes left over for other purposes.
struct RecordBlock{
    Record records[12];
    char padding[8]; //Currently there for padding purposes, we can use this for something else later

    static RecordBlock* voidToRecord(void* blockPtr){
        return (RecordBlock*)(blockPtr); //UNSAFE
    }
};

//4 Bytes to save space. Oh no help me the bytewise manipulations
class Pointer{
public:
    char entry; // Points to exact record, for dense index leaf nodes
    // We can use a negative value to indicate it does not point to data. Hence we know that it points to a tree
private:
    unsigned char block[3]; // Points to block. 0 should be a special value.
    // Block is accessed as [2],[1],[0]
    // This is private, we will use getters and setters here.
};

// Super class of blocks to check type of blocks to cast accordingly.
class block{

};

// 200B block
class treeNodeBlock{
public:
    unsigned int key[24]; // Minimum is 5, so we make 0 a special value.
    Pointer ptrs[25];
    unsigned int type;
    unsigned int getLength(){
        //TODO
    }
private:
    unsigned char block[3];
};