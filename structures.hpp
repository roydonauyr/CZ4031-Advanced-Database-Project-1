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
private:
    unsigned char block[3]; // Points to block. 0 should be a special value.
    // Block is accessed as [2],[1],[0]
    // This is private, we will use getters and setters here.
};

// 200B block
class treeNodeBlock{
    unsigned int key[24];
    Pointer ptrs[25];
    char padding[4];
};