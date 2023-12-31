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
    // Adjust size for header
    size += PAGE_MEMORY_BLOCK_HEADER_SIZE;
    uint16 bestFitOffset = findBestFitFreeMemoryBlock(size);
    char *memoryBlock = PAGE_END(buffer) - bestFitOffset;
    if(bestFitOffset == 0) {
        // No suitable free block found, attempt to get more memory
        memoryBlock = getMemoryBlock(size);
    }
    if(!memoryBlock) {
        return nullptr;
    }
    slotArray[*slotCount] = *offset;
    *slotCount += 1;
    return memoryBlock;
}

char* Page::allocateMemoryBlockAtSlot(uint16 size, uint16 index) {
    // Adjust size for header
    size += PAGE_MEMORY_BLOCK_HEADER_SIZE;
    uint16 bestFitOffset = findBestFitFreeMemoryBlock(size);
    char *memoryBlock = PAGE_END(buffer) - bestFitOffset;
    if(bestFitOffset == 0) {
        // No suitable free block found, attempt to get more memory
        memoryBlock = getMemoryBlock(size);
    }
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

char *Page::getMemoryBlock(uint16 size) {
    if(size < PAGE_MEMORY_BLOCK_HEADER_SIZE + 2) {
        // Making sure to have enough space to store 'next pointer' when block put in free list.
        size = PAGE_MEMORY_BLOCK_HEADER_SIZE + 2;
    }
    uint16 count = *slotCount;
    char *slotArrayEnd = buffer.get() + count*SLOT_SIZE + 6;
    char *pageEnd = PAGE_END(buffer);
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

void Page::deallocateMemoryBlock(uint16 index) {
    if(index >= *slotCount) {
        return;
    }
    uint16 offset = slotArray[index];
    char *pageEnd = PAGE_END(buffer);
    char *memoryBlock = pageEnd - offset;
    //Modifying Memory block Header for storing in free list.
    uint16 *nextOffset = (uint16*)(memoryBlock + 2);
    *nextOffset = *freeBlockList;
    *freeBlockList = offset;
    //Updating slot size and slot array
    uint16 n = *slotCount - 1;
    for(uint16 i = index; i < n; i++) {
        slotArray[i] = slotArray[i + 1];
    }
    *slotCount -= 1;
}

uint16 Page::findBestFitFreeMemoryBlock(uint16 size) {
    uint16 currentOffset = *freeBlockList;
    uint16 selectedOffset = 0;
    uint16 sizeDifference = MAX_INT_16;
    uint16 previousOffset = 0;
    uint16 selectedPreviousOffset = 0;
    char *pageEnd = PAGE_END(buffer);
    // Finding best fit memory block offset
    while (currentOffset != 0) {
        char *memoryBlock = pageEnd - currentOffset;
        // Access the size and next offset stored in the block header
        uint16 *currentSize = (uint16*) memoryBlock;
        uint16 *nextOffset = (uint16*) (memoryBlock + PAGE_MEMORY_BLOCK_HEADER_SIZE);
        uint16 currentSizeDifference = *currentSize - size;
        if(*currentSize >= size && sizeDifference > currentSizeDifference) {
            sizeDifference = currentSizeDifference;
            selectedOffset = currentOffset;
            selectedPreviousOffset = previousOffset;
        }
        previousOffset = currentOffset;
        currentOffset = *nextOffset;
    }
    if(selectedOffset == 0) {
        return 0;
    }
    // Update free list
    if(*freeBlockList == selectedOffset) {
        *freeBlockList = 0;
    } else {
        char *selectedMemoryBlock = pageEnd - selectedOffset;
        char *previousMemoryBlock = pageEnd - selectedPreviousOffset;
        uint16 *previousNextOffset = (uint16*) (previousMemoryBlock + PAGE_MEMORY_BLOCK_HEADER_SIZE);
        uint16 *selectedNextOffset = (uint16*) (selectedMemoryBlock + PAGE_MEMORY_BLOCK_HEADER_SIZE);
        *previousNextOffset = *selectedNextOffset;
    }
    
    return selectedOffset;
}

char* Page::getMemory(uint16 offset) {
    char *pageEnd = PAGE_END(buffer);
    return pageEnd - offset + PAGE_MEMORY_BLOCK_HEADER_SIZE;
}