#ifndef BTNODE_BUFFERPOOLMANAGER
#define BTNODE_BUFFERPOOLMANAGER
#include <memory>
#include "BufferPoolManager.h"
#include "BTNode.h"

class BTNodeBufferPoolManager {
    public:
    BTNodeBufferPoolManager(std::shared_ptr<BufferPoolManager> bufferPoolManager) {
        this->bufferPoolManager = bufferPoolManager;
    }

    BTNodeBufferPoolManager(BTNodeBufferPoolManager &manager) {
    }
    std::shared_ptr<BTNode> getNode(uint64 id);
    std::shared_ptr<BTNode> newNode();
    std::shared_ptr<BTNode> getRootPage();
    void deleteNode(uint64 id);
    void flushAll();
    private:
    std::shared_ptr<BufferPoolManager> bufferPoolManager;
};
#endif