#ifndef PAGESLOTITERATOR
#define PAGESLOTITERATOR
#include<Iterator.h>
#include<memory>
#include "PageMemory.h"
#define SLOT_SIZE 8
class PageSlotIterator: public LinearIterator<PageSlot> {
    public:
    PageSlotIterator(std::shared_ptr<char[]> buffer);
    virtual bool hasNext() override;
    virtual PageSlot getCurrentItem() override;
    virtual void next() override;
    virtual void prev() override;
    virtual PageSlot getAt(unsigned int index) override;
    virtual void setAt(unsigned int index, PageSlot item) override;
    private:
    std::shared_ptr<char[]> buffer;
    uint32 size;
    uint32 currentIndex;

};
#endif