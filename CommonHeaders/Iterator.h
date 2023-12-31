#ifndef ITERATOR
#define ITERATOR
#include "TypeDefs.h"
template<typename T>
class LinearIterator {
    public:
    virtual bool hasNext() = 0;
    virtual T getCurrentItem() = 0;
    virtual void next() = 0;
    virtual void prev() = 0;
    virtual T getAt(unsigned int index) = 0;
    virtual void setAt(unsigned int index, T item) = 0;
    virtual void reset() = 0;
    virtual uint16 count() = 0;
};
#endif