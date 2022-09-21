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