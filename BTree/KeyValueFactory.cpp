#include "KeyValueFactory.h"

KeyValueFactory::KeyValueFactory(char* buffer) {
    this->buffer = buffer;
    keySize = (uint8*)(buffer + 1);
}

std::shared_ptr<Key> KeyValueFactory::getKey() {
    // 8 is childPointer offset
    char *p = buffer + 8;
    uint8* type = (uint8*) p;
    p += 1;
    uint8* size = (uint8*) p;
    p += 1;
    switch (*type) {
        case STRING:
        {
            std::string s = std::string(p, *size);
            return std::shared_ptr<StringKey>(new StringKey(s));
        }
        case INTEGER:
        {
            uint64 keyNumber;
            memcpy(&keyNumber, p, sizeof(uint64));
            return std::shared_ptr<UIntKey>(new UIntKey(keyNumber));
        }
        default:
        assert(false && "Not yet support in Factory");
        return std::shared_ptr<StringKey>(new StringKey(std::string("")));
    }
}

std::shared_ptr<Value> KeyValueFactory::getValue() {
    // 8 is childPointer offset and 2 is keyType and keySize offset
    char *p = buffer + *keySize + 2 + 8;
    uint8* type = (uint8*) p;
    p += 1;
    uint8* size = (uint8*) p;
    p += 1;
    switch (*type) {
        case STRING_V:
        {
            std::string s = std::string(p, *size);
            return std::shared_ptr<Value>(new StringValue(s));
        }
        default:
        assert(false && "Not yet support in Factory");
        return std::shared_ptr<Value>(new StringValue(std::string("")));
    }
}