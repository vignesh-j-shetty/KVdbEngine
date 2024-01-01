#ifndef DISKMANAGER
#define DISKMANAGER
#include "StorageManagerCommon.h"
#include "Page.h"
#include <memory>
class FileManager;

class DiskManager {
    public:
    DiskManager(const char *fileName);
    ~DiskManager() throw();
    // Returns First page in linked list of used pages
    uint64 getHeadPageID();
    // Writes Page to DB file
    void writePage(Page &page);
    // returns Page with given Page ID
    std::shared_ptr<Page> readPage(uint64 id);
    
    private:
    FileManager *fileManager;
    //ID is on disk base address of page
    uint32 rootPageID;
    uint32 headFreePageID;
    char *temporaryBuffer;
    void updateHeader();
    void readHeader();
    void createRootPage();
};

#endif