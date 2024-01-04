#ifndef DATA_VALUE
#define DATA_VALUE
#include "Value.h"
#include <string>
class StringValue: public Value {
    public:
    StringValue(std::string s);
    virtual Any getValue() override;
    virtual void setValue(Any value) override;
    virtual uint8 size() override;
    virtual void serialize(void *data) override;
    private:
    std::string s;
};

#endif