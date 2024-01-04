#ifndef KEYVALUE_FACTORY
#define KEYVALUE_FACTORY
#include "Key.h"
#include "Value.h"
#include "StringKey.h"
#include "StringValue.h"
#include <memory>
#include <string>
#include <cassert>
class KeyValueFactory {
    public:
    static std::shared_ptr<Key> getKeyFromSerilizedData(char *data) {
        char *p = data;
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

};
#endif