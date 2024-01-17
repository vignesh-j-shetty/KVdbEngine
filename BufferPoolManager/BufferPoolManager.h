#ifndef BUFFERPOOLMANAGER
#define BUFFERPOOLMANAGER
#include "Page.h"
#include <memory>
#include <vector>
#include "DiskManager.h"
#include "BufferPoolFrame.h"

class BufferPoolManager {
    public:
    BufferPoolManager(DiskManager diskManager) {
        this->diskManager = diskManager;
        frameList.reserve(bufferPoolSize);
    }
    std::shared_ptr<Page> getPage(uint64 id);
    private:
    std::vector<BufferPoolFrame> frameList;
    DiskManager diskManager;
    const uint8 bufferPoolSize = 8;
    int findInBuffer(uint64 id);
};
#endif