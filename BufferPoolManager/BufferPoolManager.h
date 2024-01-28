#ifndef BUFFERPOOLMANAGER
#define BUFFERPOOLMANAGER
#define BUFFER_POOL_SIZE 10
#include "Page.h"
#include <memory>
#include <exception>
#include "DiskManager.h"
#include <vector>
class BufferPoolManager {
    public:
    BufferPoolManager(DiskManager diskManager) {
        diskManager = diskManager;
        clockHand = 0;
        frameList.reserve(BUFFER_POOL_SIZE);
    }
    std::shared_ptr<Page> getPage(uint64 id);
    std::shared_ptr<Page> newPage();
    private:
    DiskManager diskManager;
    struct PageFrame {
        std::shared_ptr<Page> pageCache;
        bool accessBit;
    };
    std::vector<PageFrame> frameList;
    uint16 clockHand;
};
#endif