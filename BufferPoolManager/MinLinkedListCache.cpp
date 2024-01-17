#include "MinLinkedListCache.h"

void MinLinkedListCache::addPageFrame(BufferPoolFrame frame) {
    std::shared_ptr<BufferPoolFrameNode> currentNode;
    {
        BufferPoolFrame frame = BufferPoolFrame(frame.page, frame.isDirty, frame.accessCount);
        currentNode->data = frame;
        currentNode->next = nullptr;
    }
    if(head == nullptr) {
        head = currentNode;
    } else {
        
    }
}

void MinLinkedListCache::removePageFrame(uint64 pageID) {

}

uint8 MinLinkedListCache::getMaxSize() {
    return 0;
}

uint8 MinLinkedListCache::getSize() {
    return 0;
}

BufferPoolFrame MinLinkedListCache::getCurrentElement() {
    BufferPoolFrame f(nullptr, true, 0);
    return f;
}

bool MinLinkedListCache::hasNext() {
    return true;
}

void MinLinkedListCache::next() {

}