#include "BufferPoolManager.h"

std::shared_ptr<Page> BufferPoolManager::getPage(uint64 id) {
    for(uint16 i = 0; i < frameList.size(); i++) {
        if(frameList[i].pageCache->getID() == id) {
            frameList[i].accessBit = true;
            return frameList[i].pageCache;
        }
    }
    std::shared_ptr<Page> page = diskManager.readPage(id);
    if(frameList.size() < BUFFER_POOL_SIZE) {
        PageFrame frame;
        frame.accessBit = true;
        frame.pageCache = page;
        frameList.push_back(frame);
    } else {
        //Clock replacement policy
        for(;;) {
            if(frameList[clockHand].accessBit) {
                frameList[clockHand].accessBit = false;
                clockHand = (clockHand + 1) % frameList.size();
            } else {
                frameList[clockHand].accessBit = true;
                if(frameList[clockHand].pageCache->isDirty()) {
                    diskManager.writePage(frameList[clockHand].pageCache);
                }
                frameList[clockHand].pageCache = page;
                break;
            }
        }
    }
    return page;
}

std::shared_ptr<Page> BufferPoolManager::newPage() {
    return diskManager.createPage();
}

std::shared_ptr<Page> BufferPoolManager::getRootPage() {
    return getPage(HEADER_SIZE);
}