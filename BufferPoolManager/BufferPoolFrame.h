#ifndef BUFFER_POOL_PAGE
#define BUFFER_POOL_PAGE
#include "Page.h"
#include "StorageManagerCommon.h"

struct BufferPoolFrame {
    std::shared_ptr<Page> page;
    uint64 accessCount;
    
    BufferPoolFrame(std::shared_ptr<Page> page, uint64 accessCount) {
        this->page = page;
        this->accessCount = accessCount;
    }

    BufferPoolFrame() {
    }

    bool operator <= (BufferPoolFrame frame) {
        return accessCount <= frame.accessCount;
    }

    
    bool operator < (BufferPoolFrame frame) {
        return accessCount < frame.accessCount;
    }

    bool operator > (BufferPoolFrame frame) {
        return accessCount > frame.accessCount;
    }
};
#endif