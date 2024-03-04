#include "BufferPoolManager.h"

std::shared_ptr<Page> BufferPoolManager::getPage(uint64 id) {
    for(uint16 i = 0; i < frameList.size(); i++) {
        if(frameList[i].pageCache->getID() == id) {
            frameList[i].accessBit = true;
            return frameList[i].pageCache;
        }
    }
    std::shared_ptr<Page> page = diskManager->readPage(id);
    addToCache(page);
    return page;
}

std::shared_ptr<Page> BufferPoolManager::newPage() {
    std::shared_ptr<Page> page = diskManager->createPage();
    addToCache(page);
    return page;
}

std::shared_ptr<Page> BufferPoolManager::getRootPage() {
    return getPage(HEADER_SIZE);
}

void BufferPoolManager::flushAll() {
    for(uint16 i = 0; i < frameList.size(); i++) {
        std::shared_ptr<Page> page = frameList[i].pageCache;
        if(page->isDirty()) {
            diskManager->writePage(page);
        }
    }
}

void BufferPoolManager::addToCache(std::shared_ptr<Page> page) {
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
                    diskManager->writePage(frameList[clockHand].pageCache);
                }
                frameList[clockHand].pageCache = page;
                break;
            }
        }
    }
}