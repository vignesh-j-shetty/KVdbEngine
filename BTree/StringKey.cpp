#include "StringKey.h"

StringKey::StringKey(std::string s) {
    this->s = s;
}

void StringKey::setData(Any any) {
    s = std::any_cast<std::string>(any);
}

Any StringKey::getData() {
    return s;
}

KeyType StringKey::getKeyType() {
    return STRING;
}

void StringKey::serialize(void *data) {
    memcpy(data, s.data(), s.size());
}

uint8 StringKey::size() {
    return s.size();
}

bool StringKey::compare(std::shared_ptr<Key> key) {
    std::string key_s = std::any_cast<std::string>(key->getData());
    return s < key_s;
}

bool StringKey::isEqual(std::shared_ptr<Key> key) {
    std::string key_s = std::any_cast<std::string>(key->getData());
    return s == key_s;
}