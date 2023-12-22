#ifndef PAGEMEMORY
#define PAGEMEMORY
#include "TypeDefs.h"
struct PageSlot {
    uint32 size;
    uint32 offset;
};
// First 8 bytes size is stored and then key,value is stored.
struct KeyValueItem {
    uint32 cellitemSize;
    uint32 keySize;
    //Key-Value pointer
    std::shared_ptr<char[]> buffer;
};

#endif