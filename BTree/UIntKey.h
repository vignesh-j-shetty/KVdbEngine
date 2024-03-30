#ifndef INT_KEY
#define INT_KEY
#include "TypeDefs.h"
#include "Key.h"

class UIntKey: public Key {
    public:
    UIntKey(uint64 key);
    virtual void setData(Any any) override;
    virtual Any getData() override;
    virtual uint8 size() override;
    virtual void serialize(void *data) override;
    virtual KeyType getKeyType() override;
    virtual bool operator<(std::shared_ptr<Key> key) override;
    virtual bool operator<=(std::shared_ptr<Key> key) override;
    virtual bool operator>(std::shared_ptr<Key> key) override;
    virtual bool operator==(std::shared_ptr<Key> key) override;
    private:
    uint64 keyValue;
};
#endif
