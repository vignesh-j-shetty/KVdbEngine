#include "BufferPoolManager.h"

int BufferPoolManager::findInBuffer(uint64 id) {
    uint8 count = frameList.size();
    for(uint8 i = 0; i < count; i++) {
        if(frameList[i].page->getID() == id) {
            return i;
        }
    }
    return -1;
}


std::shared_ptr<Page> BufferPoolManager::getPage(uint64 id) {
    int index = findInBuffer(id);
    if(index < 0) {
        
    }
}