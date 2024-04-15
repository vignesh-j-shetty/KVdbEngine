#ifndef BUFFERPOOLMANAGER
#define BUFFERPOOLMANAGER
#define BUFFER_POOL_SIZE 200
#include "Page.h"
#include <memory>
#include <exception>
#include "DiskManager.h"
#include <vector>
#include <cassert>
class BufferPoolManager {
    public:
    BufferPoolManager(std::shared_ptr<DiskManager> diskManager) {
        this->diskManager = diskManager;
        clockHand = 0;
        frameList.reserve(BUFFER_POOL_SIZE);
    }
    std::shared_ptr<Page> getPage(uint64 id);
    std::shared_ptr<Page> newPage();
    std::shared_ptr<Page> getRootPage();
    void flushAll();
    private:
    std::shared_ptr<DiskManager> diskManager;
    void addToCache(std::shared_ptr<Page> page);
    struct PageFrame {
        std::shared_ptr<Page> pageCache;
        bool accessBit;
        bool isPinned;
    };
    std::vector<PageFrame> frameList;
    uint16 clockHand;
};
#endif