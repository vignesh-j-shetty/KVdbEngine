#include "BTNodeBufferPoolManager.h"

std::shared_ptr<BTNode> BTNodeBufferPoolManager::getNode(uint64 id) {
    std::shared_ptr<Page> page = bufferPoolManager->getPage(id);
    return std::shared_ptr<BTNode>(new BTNode(page));
}

std::shared_ptr<BTNode> BTNodeBufferPoolManager::newNode() {
    std::shared_ptr<Page> page = bufferPoolManager->newPage();
    return std::shared_ptr<BTNode>(new BTNode(page));
}

std::shared_ptr<BTNode> BTNodeBufferPoolManager::getRootPage() {
    return getNode(16);
}

void BTNodeBufferPoolManager::flushAll() {
    bufferPoolManager->flushAll();
}

void BTNodeBufferPoolManager::setIsPinnnedStatus(uint64 pageID, bool pinStatus) {
    bufferPoolManager->setIsPinned(pageID, pinStatus);
}