#ifndef PAGE
#define PAGE
#include <cstring>
#include<memory>
#include "StorageManagerCommon.h"
#include "TypeDefs.h"
#include "PageSlotIterator.h"
#define SLOT_SIZE 2
#define PAGE_MEMORY_BLOCK_HEADER_SIZE 2
#define PAGE_HEADER_SIZE 8
#define PAGE_END(buffer) (buffer.get() + DISKMANAGER_PAGESIZE)

enum PageType {
    BT_ROOT_PAGE = 0, BT_INTERNAL_NODE = 1, BT_LEAF_NODE = 2, OVERFLOW = 3
};

class DiskManager;
/*
Page Header Format
Page type - 2
Free List - 2
Offset    - 2
Count     - 2
*/

class Page {
    public:
    Page(char *_buffer, uint64 pageID) {
        buffer = std::shared_ptr<char[]>(_buffer);
        id = pageID;
        mapPointers();
    }
    // Copy Constructor
    Page(Page &page) {
        buffer = page.buffer;
        id = page.id;
        mapPointers();
    }

    uint64 getID() {
        return id;
    }
    // Allocates space to store Key/Value within Page
    char *allocateMemoryBlock(uint16 size);
    // Allocates space at particular slot index
    char *allocateMemoryBlockAtSlot(uint16 size, uint16 index);
    // DeAllocates space of given slot index
    void dellocateMemoryBlock(uint16 index);
    uint16 findBestFitFreeMemoryBlock(uint16 size);
    private:
    // This buffer will shared while copying.
    std::shared_ptr<char[]> buffer;
    uint64 id;
    //Headers
    uint16 *pageType;
    uint16 *freeBlockList;
    uint16 *offset;
    uint16 *slotCount;

    uint16* slotArray;
    friend class DiskManager;
    void mapPointers();
    // Create Memory Block With Required Header, does not update slot array
    char *getMemoryBlock(uint16 size);
    
};
#endif