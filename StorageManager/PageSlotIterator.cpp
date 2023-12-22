#include "PageSlotIterator.h"

PageSlotIterator::PageSlotIterator(std::shared_ptr<char[]> buffer) {
    uint32 *size = (uint32*)buffer.get();
    this->size = *size;
    this->buffer = buffer;
    this->currentIndex = 0;
}

bool PageSlotIterator::hasNext() {
    return currentIndex < size;
}

PageSlot PageSlotIterator::getCurrentItem() {
    char *currentSlotAddress = buffer.get() + SLOT_SIZE * currentIndex;
    uint32 *temp = (uint32*)currentSlotAddress;
    PageSlot slot;
    slot.size = *temp;
    temp += 4;
    slot.offset = *temp;
    return slot;
}

void PageSlotIterator::next() {
    if (currentIndex == size - 1) {
        return;
    }
    currentIndex++;
}

void PageSlotIterator::prev() {
    if(currentIndex == 0) {
        return;
    }
    currentIndex--;
}

PageSlot PageSlotIterator::getAt(unsigned int index) {
    PageSlot slot;
    return slot;
}

void PageSlotIterator::setAt(unsigned int index, PageSlot item) {

}