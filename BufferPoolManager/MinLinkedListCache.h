#ifndef MIN_LINKED_LIST_CACHE
#define MIN_LINKED_LIST_CACHE
#include "BufferPoolPageFrameCache.h"
#include <memory>
class MinLinkedListCache: public BufferPoolPageFrameCache {
    public:
    virtual void addPageFrame(BufferPoolFrame frame) override;
    virtual void removePageFrame(uint64 pageID) override;
    virtual uint8 getMaxSize() override;
    virtual uint8 getSize() override;
    virtual BufferPoolFrame getCurrentElement() override;
    virtual bool hasNext() override;
    virtual void next() override;
    private:
    struct BufferPoolFrameNode {
        BufferPoolFrame data;
        std::shared_ptr<BufferPoolFrameNode> next;
    };
    std::shared_ptr<BufferPoolFrameNode> head; 
};

#endif