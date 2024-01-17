#ifndef BUFFER_POOL_PAGE_FRAME_CACHE
#define BUFFER_POOL_PAGE_FRAME_CACHE
#include "BufferPoolFrame.h"

class BufferPoolPageFrameCache {
    public:
    virtual void addPageFrame(BufferPoolFrame frame) = 0;
    virtual void removePageFrame(uint64 pageID) = 0;
    virtual uint8 getMaxSize() = 0;
    virtual uint8 getSize() = 0;
    virtual BufferPoolFrame getCurrentElement() = 0;
    virtual bool hasNext() = 0;
    virtual void next() = 0;
};


#endif
