#include "UIntKey.h"

UIntKey::UIntKey(uint64 key) {
    this->keyValue = key;
}

void UIntKey::setData(Any any) {
    keyValue = std::any_cast<uint64>(any);
}

Any UIntKey::getData() {
    return keyValue;
}

KeyType UIntKey::getKeyType() {
    return INTEGER;
}

void UIntKey::serialize(void *data) {
    memcpy(data, &keyValue, sizeof(uint64));
}

uint8 UIntKey::size() {
    return sizeof(uint64);
}

bool UIntKey::operator<(std::shared_ptr<Key> key) {
    uint64 _key = std::any_cast<uint64>(key->getData());
    return keyValue < _key;
}

bool UIntKey::operator<=(std::shared_ptr<Key> key) {
    uint64 _key = std::any_cast<uint64>(key->getData());
    return keyValue <= _key;
}

bool UIntKey::operator>(std::shared_ptr<Key> key) {
    uint64 _key = std::any_cast<uint64>(key->getData());
    return keyValue > _key;
}

bool UIntKey::operator==(std::shared_ptr<Key> key) {
    uint64 _key = std::any_cast<uint64>(key->getData());
    return keyValue == _key;
}