#ifndef VALUE
#define VALUE
#include "TypeDefs.h"
class Value {
    public:
    virtual Any getValue() = 0;
    virtual void setValue(Any value) = 0;
    virtual uint8 size() = 0;
    virtual void serialize(void *data) = 0;
};
#endif