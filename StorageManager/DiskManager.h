#ifndef DISKMANAGER
#define DISKMANAGER
#define DISKMANAGER_HEADERSIZE 20
#define DISKMANAGER_POINTERSIZE 8
#define DISKMANAGER_PAGESIZE 2024
#include "FileManager.h"
#include "UnixFileManager.h"
#include "WindowsFileManager.h"
#include "DiskManager.h"

class DiskManager {
    public:
    DiskManager(const char *fileName);
    ~DiskManager() throw();
    //Returns page size that is used in current file
    uint32 getPageSize();
    //Returns First page in linked list of used pages
    uint64 getHeadPageID();
    //Returns First page in linked list of free pages
    uint64 getHeadFreePageID();
    //Reads Page from disk given page id
    void readPage(uint64 pageID, char *buffer);
    //Writes Page in page of given Page
    void writePage(uint64 pageID, char *buffer);
    //Creates new Page and adds to head of free list in Disk
    void createFreePage();
    //Gets Page From Free List and adds to Owned Page List
    void createOwnedPage();
    private:
    FileManager *fileManager;
    void updateHeader();
    uint32 pageSize;
    //ID is on disk base address of page
    uint64 headOwnedPageID;
    uint64 headFreePageID;
};

#endif