#include "BTNodeBufferPoolManager.h"

BTNode BTNodeBufferPoolManager::getNode(uint64 id) {
    std::shared_ptr<Page> page = bufferPoolManager.getPage(id);
    return BTNode(page);
}

BTNode BTNodeBufferPoolManager::newNode() {
    std::shared_ptr<Page> page = bufferPoolManager.newPage();
    return BTNode(page);
}

BTNode BTNodeBufferPoolManager::getRootPage() {
    return getNode(16);
}

