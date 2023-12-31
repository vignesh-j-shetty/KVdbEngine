#include "PageSlotIterator.h"

PageSlotIterator::PageSlotIterator(uint16 *slotArray, uint16 *slotArrayCount) {
    this->slotArray = slotArray;
    this->slotArrayCount = slotArrayCount;
    currentIndex = 0;
}

bool PageSlotIterator::hasNext() {
    return currentIndex < *slotArrayCount;
}

uint16 PageSlotIterator::getCurrentItem() {
    return slotArray[currentIndex];
}

void PageSlotIterator::next() {
    if(currentIndex < *slotArrayCount) {
        currentIndex++;
    }
}

void PageSlotIterator::prev() {
    if (currentIndex > 0) {
        return;
    }
    currentIndex--;
}

uint16 PageSlotIterator::getAt(unsigned int index) {
    return slotArray[index];
}

void PageSlotIterator::setAt(unsigned int index, uint16 item) {
    slotArray[index] = item;
}

void PageSlotIterator::reset() {
    currentIndex = 0;
}

uint16 PageSlotIterator::count() {
    return *slotArrayCount;
}