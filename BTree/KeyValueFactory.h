#ifndef KEYVALUE_FACTORY
#define KEYVALUE_FACTORY
#include "Key.h"
#include "Value.h"
#include "StringKey.h"
#include "StringValue.h"
#include <memory>
#include <string>
#include <cassert>
// Only to be used in BTNode.
class KeyValueFactory {
    public:
    KeyValueFactory(char *buffer);
    KeyValueFactory() {
        buffer = nullptr;
    }
    std::shared_ptr<Key> getKey();
    std::shared_ptr<Value> getValue();
    private:
    char *buffer;
    uint8 *keySize;

};
#endif