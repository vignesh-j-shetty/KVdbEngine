#ifndef KEY
#define KEY
#include "TypeDefs.h"
#include<memory>
enum KeyType {
    STRING = 0, INTEGER = 1
};
class Key {
    public:
    virtual void setData(Any any) = 0;
    virtual Any getData() = 0;
    virtual uint8 size() = 0;
    virtual void serialize(void *data) = 0;
    virtual KeyType getKeyType() = 0;
    virtual bool operator<(std::shared_ptr<Key> key) = 0;
    virtual bool operator<=(std::shared_ptr<Key> key) = 0;
    virtual bool operator>(std::shared_ptr<Key> key) = 0;
    virtual bool operator==(std::shared_ptr<Key> key) = 0;
};

#endif