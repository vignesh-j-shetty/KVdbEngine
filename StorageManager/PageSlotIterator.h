#ifndef PAGESLOTITERATOR
#define PAGESLOTITERATOR
#include<Iterator.h>
#include<memory>
#include "PageMemory.h"
class PageSlotIterator: public LinearIterator<uint16> {
    public:
    PageSlotIterator(uint16 *slotArray, uint16 *slotArrayCount);
    virtual bool hasNext() override;
    virtual uint16 getCurrentItem() override;
    virtual void next() override;
    virtual void prev() override;
    virtual uint16 getAt(unsigned int index) override;
    virtual void setAt(unsigned int index, uint16 item) override;
    virtual void reset() override;
    virtual uint16 count() override;
    private:
    uint16 *slotArray;
    uint16 *slotArrayCount;
    uint16 currentIndex;
};
#endif