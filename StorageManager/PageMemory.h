#ifndef PAGEMEMORY
#define PAGEMEMORY
#include "TypeDefs.h"
#define KV_ITEM_HEADER_SIZE 3

/*
Header Info
Total Size - 2
Key Size - 1
*/
/*
So Total Header size is 3 bytes
*/
struct KeyValueItem {
    public:
    uint8 keySize;
    uint16 getSize() {
        return itemSize;
    }
    void copyDataToPage(char *_buffer) {
        memcpy(buffer + KV_ITEM_HEADER_SIZE, _buffer, itemSize - KV_ITEM_HEADER_SIZE);
    }
    private:
    //Key-Value pointer to Page buffer
    char *buffer;
    //Including Header size
    uint16 itemSize;

};

#endif