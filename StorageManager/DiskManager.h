#ifndef DISKMANAGER
#define DISKMANAGER
#define HEADER_SIZE 16
#include "StorageManagerCommon.h"
#include "Page.h"
#include <memory>
class FileManager;

class DiskManager {
    public:
    DiskManager() {
        
    }
    DiskManager(const char *fileName);
    DiskManager(DiskManager &diskManager);
    ~DiskManager() throw();
    // Returns First page in linked list of used pages
    uint64 getHeadPageID();
    // Writes Page to DB file
    void writePage(std::shared_ptr<Page> page);
    // returns Page with given Page ID
    std::shared_ptr<Page> readPage(uint64 id);
    std::shared_ptr<Page> createPage();
    void deletePage(uint64 id);
    private:
    struct Header {
        uint64 rootPageID;
        uint64 freeHeadPageID;
    };
    std::shared_ptr<FileManager> fileManager;
    //ID is on disk base address of page
    char *temporaryBuffer;
    void fillPageNULL(char *buffer);
    uint64 createNewPage();
    void write(Header header);
    void read(Header &header);
    void setFreePageNextPageID(uint64 id, uint64 nextPageID);
    uint64 getFreePageNextPageID(uint64 id);
};

#endif