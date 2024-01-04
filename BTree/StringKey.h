#ifndef STRINGKEY
#define STRINGKEY
#include "Key.h"
#include<string>
class StringKey: public Key {
    public:
    StringKey(std::string s);
    virtual void setData(Any any) override;
    virtual Any getData() override;
    virtual uint8 size() override;
    virtual void serialize(void *data) override;
    virtual KeyType getKeyType() override;
    virtual bool compare(std::shared_ptr<Key> key) override;
    private:
    std::string s;
};

#endif