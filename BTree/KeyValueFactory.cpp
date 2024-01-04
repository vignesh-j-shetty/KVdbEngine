#include "KeyValueFactory.h"

KeyValueFactory::KeyValueFactory(char* buffer) {
    this->buffer = buffer;
    keySize = (uint8*)(buffer + 1);
}

std::shared_ptr<Key> KeyValueFactory::getKey() {
    char *p = buffer;
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
        default:
        assert(false && "Not yet support in Factory");
    }
}

std::shared_ptr<Value> KeyValueFactory::getValue() {
    char *p = buffer + *keySize + 2;
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
    }
}