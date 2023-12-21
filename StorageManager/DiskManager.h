#ifndef DISKMANAGER
#define DISKMANAGER
#include "StorageManagerCommon.h"
#include "Page.h"

class FileManager;

class DiskManager {
    public:
    DiskManager(const char *fileName);
    ~DiskManager() throw();
    // Returns First page in linked list of used pages
    uint64 getHeadPageID();
    // Returns First page in linked list of free pages
    uint64 getHeadFreePageID();
    // Writes Page to DB file
    void writePage(Page &page);
    // returns Page with given Page ID
    Page readPage(uint64 id);
    
    private:
    FileManager *fileManager;
    //ID is on disk base address of page
    uint64 headAllocatedPageID;
    uint64 headFreePageID;
    char *temporaryBuffer;
    void updateHeader();
};

#endif