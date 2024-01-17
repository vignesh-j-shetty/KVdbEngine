#ifndef BUFFER_POOL_PAGE
#define BUFFER_POOL_PAGE
#include "Page.h"
#include "StorageManagerCommon.h"

struct BufferPoolFrame {
    std::shared_ptr<Page> page;
    bool isDirty;
    uint64 accessCount;
    
    BufferPoolFrame(std::shared_ptr<Page> page, bool isDirty, uint64 accessCount) {
        this->page = page;
        this->isDirty = isDirty;
        this->accessCount = accessCount;
    }
};
#endif