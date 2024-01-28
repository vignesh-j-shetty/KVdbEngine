#ifndef PAGE
#define PAGE
#include <cstring>
#include<memory>
#include "StorageManagerCommon.h"
#include "TypeDefs.h"
#define SLOT_SIZE 2
#define PAGE_RECORD_HEADER_SIZE 2
#define PAGE_FREE_RECORD_HEADER_SIZE 4
#define PAGE_HEADER_SIZE 7
#define PAGE_END (buffer + DISKMANAGER_PAGESIZE)

enum PageType {
    BT_ROOT_PAGE = 0, BT_INTERNAL_PAGE = 1, BT_LEAF_PAGE = 2, _OVERFLOW = 3
};

class DiskManager;
/*
Page Header Format
Page type - 1 byte
Free List - 2 bytes
Offset    - 2 bytes
Count     - 2 byte
Offset List - 2 bytes each
*/

class Page {
    public:
    // Only should be accessed in DiskManager
    Page(char *_buffer, uint64 pageID);
    Page() {
        //Empty do nothing
    }
    // Copy Constructor
    Page(Page &page);
    ~Page() {
        delete[] buffer;
    }
    inline uint64 getID() {
        return id;
    }
    inline bool isDirty() {
        return _isDirty;
    }
    // Gets number of record
    uint16 getRecordCount();
    //Gets Record size
    uint16 getRecordSize(uint16 index);
    // Inserts Record data into the page, creating a new slot and managing free space.
    void insertRecord(char *data, uint16 dataLength, uint16 atIndex);
    // Adds record at end of slot list
    void insertRecord(char *data, uint16 dataLength);
    // Reads record for given number of bytes in dataLength into data
    void readRecord(char *data, uint16 dataLength, uint16 atIndex);
    // Removes record at given slot Index
    void removeRecord(uint16 atIndex);
    private:
    char *buffer = nullptr;
    uint64 id;
    //Headers
    struct PageHeaders {
        uint8 *pageType = nullptr;
        uint16 *freeBlockList = nullptr;
        uint16 *freeSpaceOffset = nullptr;
        uint16 *slotCount = nullptr;
        uint16 *slotList = nullptr;
    };
    PageHeaders header;
    uint16* slotArray;
    friend class DiskManager;
    //Functions
    void mapPointers();
    void allocateSpace(uint16 size, uint16 atIndex);
    void addBlockToFreeList(uint16 atIndex);
    char *getRecordPointer(uint16 index);
    bool _isDirty = false;
};
#endif