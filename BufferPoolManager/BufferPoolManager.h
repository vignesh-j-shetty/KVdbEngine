#ifndef BUFFERPOOLMANAGER
#define BUFFERPOOLMANAGER
#include "Page.h"
#include <memory>
#include <exception>
#include "DiskManager.h"
#include "BufferPoolFrame.h"
#include "BufferPoolPageFrameCache.h"
#include "MinHeapFrameCache.h"
class BufferPoolManager {
    public:
    BufferPoolManager(DiskManager diskManager) {
        this->diskManager = diskManager;
    }

    std::shared_ptr<Page> getPage(uint64 id);
    std::shared_ptr<Page> newPage();
    void pinPage(uint64 id);
    void unPinPage(uint64 id);
    private:
    DiskManager diskManager;
};
#endif