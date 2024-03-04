#include "Page.h"
#include<iostream>
#include <cassert>
#include "NoSpaceException.h"

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
    header.slotCount = (uint16 *)(buffer + 5);
    header.slotList = (uint16 *) (buffer + 7);
    slotArray = (uint16*) (buffer + PAGE_HEADER_SIZE);
}

void Page::allocateSpace(uint16 size, uint16 atIndex) {
    size += PAGE_RECORD_HEADER_SIZE;
    if(size < PAGE_FREE_RECORD_HEADER_SIZE) {
        size = PAGE_FREE_RECORD_HEADER_SIZE;
    }
    uint16 adjustedOffset = *(header.freeSpaceOffset) + size;
    char *adjustedSpace  = PAGE_END - adjustedOffset;
    {
        uint16 slotCount = *(header.slotCount);
        char *nextSlotEndPosition  = (char *)(slotArray  + slotCount + 1);
        if(nextSlotEndPosition > adjustedSpace) {
            std::cout<<"No Available free Space\n";
            return throw NoSpaceException();
        }
    }
    *(header.freeSpaceOffset) = adjustedOffset;
    //Update slotList
    uint16 *slotList = header.slotList;
    uint16 *slotCount = header.slotCount;
    assert(atIndex <= *slotCount && "Invalid Index given to Page::allocateSpace");
    for(uint16 i = *slotCount; i > atIndex; i--) {
        slotList[i] = slotList[i - 1];
    }
    uint16* sizeHeader = (uint16*) adjustedSpace;
    *sizeHeader = size;
    *slotCount += 1;
    slotList[atIndex] = *(header.freeSpaceOffset);
}

inline char* Page::getRecordPointer(uint16 index) {
    assert(index < *(header.slotCount));
    uint16 offset = header.slotList[index];
    return PAGE_END - offset + PAGE_RECORD_HEADER_SIZE;
}

uint16 Page::getRecordSize(uint16 index) {
    assert(index < *(header.slotCount));
    uint16 offset = header.slotList[index];
    uint16* size = (uint16*) (PAGE_END - offset);
    return *size - PAGE_RECORD_HEADER_SIZE;
}

uint16 Page::getRecordCount() {
    return *(header.slotCount);
}

void Page::insertRecord(char *data, uint16 dataLength, uint16 atIndex) {
    try {
        //Allocate space at end
        allocateSpace(dataLength, atIndex);
        
    } catch(NoSpaceException error) {
        throw error;
    }
    char* record = getRecordPointer(atIndex);
    memcpy(record, data, dataLength);
    _isDirty = true;

}

void Page::insertRecord(char *data, uint16 dataLength) {
    uint16 currentIndex = *(header.slotCount);
    insertRecord(data, dataLength, currentIndex);
}

void Page::readRecord(char *data, uint16 dataLength, uint16 atIndex) {
    assert(atIndex < *(header.slotCount));
    memcpy(data, getRecordPointer(atIndex), dataLength);
}

void Page::removeRecord(uint16 atIndex) {
    addBlockToFreeList(atIndex);
    //Updating Slot Array
    uint16 count = *(header.slotCount) - 1;
    for (uint16 i = atIndex; i < count; i++) {
        slotArray[i] = slotArray[i + 1];
    }
    *(header.slotCount) -= 1;
    _isDirty = true;
}

void Page::addBlockToFreeList(uint16 atIndex) {
    assert(atIndex < *(header.slotCount));
    uint16 removeOffset = header.slotList[atIndex];
    if(removeOffset == *(header.freeSpaceOffset)) {
        *(header.freeSpaceOffset) = removeOffset - getRecordSize(atIndex) - PAGE_RECORD_HEADER_SIZE;
    } else {
        uint16* nextPointer = (uint16*) getRecordPointer(atIndex);
        *nextPointer = *(header.freeBlockList);
        *(header.freeBlockList) = removeOffset;
    }
}

void Page::setPageType(PageType type) {
    *(header.pageType) = type;
    _isDirty = true;
}


void Page::updateRecord(char *data, uint16 index) {
    uint16 size = getRecordSize(index);
    char* record = getRecordPointer(index);
    memcpy(record, data, size);
    _isDirty = true;
}

PageType Page::getPageType() {

    switch (*(header.pageType)) {
        case 0:
        return BT_ROOT_PAGE;
        case 1:
        return BT_INTERNAL_PAGE;
        case 2:
        return BT_LEAF_PAGE;
        default:
        return _OVERFLOW;
    }
}
