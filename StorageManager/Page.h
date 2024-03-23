#ifndef PAGE
#define PAGE
#include <cstring>
#include<memory>
#include "StorageManagerCommon.h"
#include "TypeDefs.h"
#include "NoSpaceException.h"
#define SLOT_SIZE 2
#define PAGE_RECORD_HEADER_SIZE 2
#define PAGE_FREE_RECORD_HEADER_SIZE 4
#define PAGE_HEADER_SIZE 15
#define PAGE_END (buffer + DISKMANAGER_PAGESIZE)
#define MAX_RECORD_SIZE 50
#define OTHER_DATA_SIZE 8
enum PageType {
    BT_ROOT_PAGE = 0, BT_INTERNAL_PAGE = 1, BT_LEAF_PAGE = 2, _OVERFLOW = 3
};

class DiskManager;
class BTNode;
/*
Page Header Format   
Type         Size          Offset
Page type -  1 byte           0
Other Data - 8 bytes          1
Free List -  2 bytes          9
Offset    -  2 bytes          11
Count     -  2 byte           13
Offset List -2 bytes each     15
*/
class Page {
    public:
    // Only should be accessed in DiskManager
    Page(char *_buffer, uint64 pageID);
    Page() {
        buffer = nullptr;
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
    // Edits record's content
    void updateRecord(char *data, uint16 index);
    // Sets the page type
    void setPageType(PageType type);
    // Reads other miscellaneous data in header of 8 bytes
    void readOtherData(char *data);
    // Updates other miscellaneous data in header of 8 bytes
    void updateOtherData(char *data);
    // Gets the page type
    PageType getPageType();
    // Compacts memory
    void compactSpace();

    private:
    char *buffer = nullptr;
    uint64 id;
    //Headers
    struct PageHeaders {
        uint8 *pageType = nullptr;
        char *otherData = nullptr;
        uint16 *freeBlockList = nullptr;
        uint16 *freeSpaceOffset = nullptr;
        uint16 *slotCount = nullptr;
        uint16 *slotList = nullptr;
    };
    PageHeaders header;
    uint16* slotArray;
    friend class DiskManager;
    friend class BTNode;
    //Functions
    void mapPointers();
    void allocateSpace(uint16 size, uint16 atIndex);
    void addBlockToFreeList(uint16 atIndex);
    char *getRecordPointer(uint16 index);
    bool _isDirty = false;
};
#endif