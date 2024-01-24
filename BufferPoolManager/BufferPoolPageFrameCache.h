#ifndef BUFFER_POOL_PAGE_FRAME_CACHE
#define BUFFER_POOL_PAGE_FRAME_CACHE
#include "BufferPoolFrame.h"

class BufferPoolPageFrameCache {
    public:
    virtual void addPageFrame(BufferPoolFrame frame) = 0;
    virtual BufferPoolFrame removePageFrame() = 0;
    virtual BufferPoolFrame getFrame(uint64 id) = 0;
};

class BufferPoolPageFrameCacheException {
    public:
    static const uint8 PAGE_NOT_IN_CACHE = 1;
    static const uint8 CACHE_MEMORY_FULL = 2;
    static const uint8 INVALID_OPERATION = 3;
    BufferPoolPageFrameCacheException(uint8 type) {
        exceptionType = type;
    }
    uint8 getExceptionType() {
        return exceptionType;
    }
    private:
    uint8 exceptionType;
};

#endif
