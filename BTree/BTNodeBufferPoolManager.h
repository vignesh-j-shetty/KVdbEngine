#ifndef BTNODE_BUFFERPOOLMANAGER
#define BTNODE_BUFFERPOOLMANAGER
#include <memory>
#include "BufferPoolManager.h"
#include "BTNode.h"

class BTNodeBufferPoolManager {
    public:
    BTNodeBufferPoolManager(BufferPoolManager bufferPoolManager) {
        this->bufferPoolManager = bufferPoolManager;
    }

    BTNodeBufferPoolManager(BTNodeBufferPoolManager &manager) {
    }
    BTNode getNode(uint64 id);
    BTNode newNode();
    BTNode getRootPage();
    private:
    BufferPoolManager bufferPoolManager;
};
#endif