#ifndef VALUE
#define VALUE
#include "TypeDefs.h"

enum ValueType {
    STRING_V = 0, INTEGER_V = 1
};

class Value {
    public:
    virtual Any getValue() = 0;
    virtual void setValue(Any value) = 0;
    virtual uint8 size() = 0;
    virtual void serialize(void *data) = 0;
    virtual ValueType getType() = 0;
};
#endif