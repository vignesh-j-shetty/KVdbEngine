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

bool StringKey::operator<(std::shared_ptr<Key> key) {
    std::string _s = std::any_cast<std::string>(key->getData());
    bool res  = s < _s;
    return res;
}

bool StringKey::operator<=(std::shared_ptr<Key> key) {
    std::string _s = std::any_cast<std::string>(key->getData());
    bool res  = s < _s;
    return res;
}

bool StringKey::operator>(std::shared_ptr<Key> key) {
    std::string _s = std::any_cast<std::string>(key->getData());
    bool res  = s > _s;
    return res; 
}

bool StringKey::operator==(std::shared_ptr<Key> key) {
    std::string _s = std::any_cast<std::string>(key->getData());
    bool res  = s == _s;
    return res;
}