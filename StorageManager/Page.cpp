#include "Page.h"
#include<iostream>
#include <cassert>
Page::Page(Page &page) {
    buffer = page.buffer;
    id = page.id;
    buffer = new char[DISKMANAGER_PAGESIZE];
    memcpy(buffer, page.buffer, DISKMANAGER_PAGESIZE);
    mapPointers();
    *(header.freeSpaceOffset) = 0;
}

Page::Page(char *_buffer, uint64 pageID) {
    assert(_buffer != nullptr);
    buffer = _buffer;
    id = pageID;
    mapPointers();
}

void Page::mapPointers() {
    header.pageType = (uint8 *)buffer;
    header.freeBlockList = (uint16*)(buffer + 1);
    header.freeSpaceOffset = (uint16 *)(buffer + 3);
    header.slotCount = (uint8 *)(buffer + 5);
    header.slotList = (uint16 *) (buffer + 6);
    slotArray = (uint16*) (buffer + PAGE_HEADER_SIZE);
}

void Page::allocateSpace(uint16 size, uint8 atIndex) {
    size += PAGE_RECORD_HEADER_SIZE;
    if(size < PAGE_FREE_RECORD_HEADER_SIZE) {
        size = PAGE_FREE_RECORD_HEADER_SIZE;
    }
    uint16 adjustedOffset = *(header.freeSpaceOffset) + size;
    char *adjustedSpace  = PAGE_END - adjustedOffset;
    {
        uint8 slotCount = *(header.slotCount);
        char *nextSlotEndPosition  = (char *)(slotArray  + slotCount + 1);
        if(nextSlotEndPosition > adjustedSpace) {
            std::cout<<"No Available free Space";
            return throw;
        }
    }
    *(header.freeSpaceOffset) = adjustedOffset;
    //Update slotList
    uint16 *slotList = header.slotList;
    uint8 *slotCount = header.slotCount;
    assert(atIndex <= *slotCount && "Invalid Index given to Page::allocateSpace");
    for(uint8 i = *slotCount; i > atIndex; i--) {
        slotList[i] = slotList[i - 1];
    }
    uint16* sizeHeader = (uint16*) adjustedSpace;
    *sizeHeader = size;
    *slotCount += 1;
    slotList[atIndex] = *(header.freeSpaceOffset);
}

inline char* Page::getRecordPointer(uint8 index) {
    assert(index < *(header.slotCount));
    uint16 offset = header.slotList[index];
    return PAGE_END - offset + PAGE_RECORD_HEADER_SIZE;
}

uint16 Page::getRecordSize(uint8 index) {
    assert(index < *(header.slotCount));
    uint16 offset = header.slotList[index];
    uint16* size = (uint16*) (PAGE_END - offset);
    return *size - 2;
}

uint8 Page::getRecordCount() {
    return *(header.slotCount);
}

void Page::insertRecord(char *data, uint16 dataLength, uint8 atIndex) {
    try {
        //Allocate space at end
        allocateSpace(dataLength, atIndex);
        char* record = getRecordPointer(atIndex);
        memcpy(record, data, dataLength);
    } catch(...) {
        std::cout<<"Error while allocating error";
    }
}

void Page::insertRecord(char *data, uint16 dataLength) {
    uint8 currentIndex = *(header.slotCount);
    insertRecord(data, dataLength, currentIndex);
}

void Page::readRecord(char *data, uint16 dataLength, uint8 atIndex) {
    assert(atIndex < *(header.slotCount));
    memcpy(data, getRecordPointer(atIndex), dataLength);
}

void Page::removeRecord(uint8 atIndex) {
    assert(atIndex < *(header.slotCount));
    uint16 removeOffset = header.slotList[atIndex];
    uint16* nextPointer = (uint16*) getRecordPointer(atIndex);
    *nextPointer = *(header.freeBlockList);
    *(header.freeBlockList) = removeOffset;
    //Updating Slot Array
    uint8 count = *(header.slotCount) - 1;
    for (uint8 i = atIndex; i < count; i++) {
        slotArray[i] = slotArray[i + 1];
    }
    *(header.slotCount) -= 1;
}