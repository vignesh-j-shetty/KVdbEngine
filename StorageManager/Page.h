#ifndef PAGE
#define PAGE
#include <cstring>
#include<memory>
#include "StorageManagerCommon.h"
#include "TypeDefs.h"
#include "PageSlotIterator.h"

class DiskManager;
class Page {
    public:
    Page(char *_buffer, uint64 pageID) {
        buffer = std::shared_ptr<char[]>(_buffer);
        id = pageID;
    }
    // Copy Constructor
    Page(Page &page) {
        buffer = page.buffer;
        id = page.id;
    }

    uint64 getID() {
        return id;
    }
    private:
    // This buffer will shared while copying.
    std::shared_ptr<char[]> buffer;
    uint64 id;
    friend class DiskManager;
};
#endif