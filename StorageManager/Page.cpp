#include "Page.h"
#include<iostream>

void Page::mapPointers() {
    char *p = buffer.get();
    pageType = (uint16*)p;
    p += 2;
    freeBlockList = (uint16*)p;
    p += 2;
    offset = (uint16*)p;
    p += 2;
    slotCount = (uint16*)p;
    p += 2;
    slotArray = (uint16*)p;
}

char* Page::allocateMemoryBlock(uint16 size) {
    char *memoryBlock = getMemoryBlock(size);
    if(!memoryBlock) {
        return nullptr;
    }
    slotArray[*slotCount] = *offset;
    *slotCount += 1;
    return memoryBlock;
}

char* Page::allocateMemoryBlockAtSlot(uint16 size, uint16 index) {
    char *memoryBlock = getMemoryBlock(size);
    if(!memoryBlock) {
        return nullptr;
    }
    *slotCount += 1;
    for (uint16 i = *slotCount; i > index; i--) {
        slotArray[i] = slotArray[i-1];
    }
    slotArray[index] = *offset;
    return memoryBlock;
}

char *Page::getMemoryBlock(uint16 size){
    size += PAGE_MEMORY_BLOCK_HEADER_SIZE;
    if(size < PAGE_MEMORY_BLOCK_HEADER_SIZE + 2) {
        // Making sure to have enough space to store 'next pointer' when block put in free list.
        size = PAGE_MEMORY_BLOCK_HEADER_SIZE + 2;
    }
    uint16 count = *slotCount;
    char *slotArrayEnd = buffer.get() + count*SLOT_SIZE + 6;
    char *pageEnd = buffer.get() + DISKMANAGER_PAGESIZE;
    uint16 allocatedOffset = *offset + size;
    //Check if space available
    if(slotArrayEnd + SLOT_SIZE >= pageEnd - allocatedOffset) {
        std::cout<<"No Space Available"<<std::endl;
        return nullptr;
    }
    //Update Offset
    *offset = allocatedOffset;
    char *memoryBlock = pageEnd - *offset;
    uint16 *memoryBlockHeader = (uint16*) memoryBlock;
    *memoryBlockHeader = size;
    return memoryBlock + PAGE_MEMORY_BLOCK_HEADER_SIZE;
}

void Page::dellocateMemoryBlock(uint32 offset) {
    //TODO
}