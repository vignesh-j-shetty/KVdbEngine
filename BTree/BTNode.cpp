#include "BTNode.h"
#include<iostream>
#include <cassert>
#include<string.h>

BTNode::BTNode(std::shared_ptr<Page> page) {
    this->page = page;
    temporaryRecordBuffer = new char[RECORD_MAX_SIZE];
    kvFactory = KeyValueFactory(temporaryRecordBuffer);
}

void BTNode::insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint8 count = page->getRecordCount();

    if(count == 0) {
        uint16 totalSize = serializeToTemporaryBuffer(key, value);
        page->insertRecord(temporaryRecordBuffer, totalSize);
        return;
    }
    for(uint8 i = 0; i < count; i++) {
        page->readRecord(temporaryRecordBuffer, page->getRecordSize(i), i);
        std::shared_ptr<Key> _key = kvFactory.getKey();
        if(key->compare(_key)) {
            uint16 totalSize = serializeToTemporaryBuffer(key, value);
            page->insertRecord(temporaryRecordBuffer, totalSize, i);
            return;
        }
    }
    uint16 totalSize = serializeToTemporaryBuffer(key, value);
    page->insertRecord(temporaryRecordBuffer, totalSize, count);
}


std::shared_ptr<Key> BTNode::getKey(uint8 index) {
    assert(index <= page->getRecordCount());
    page->readRecord(temporaryRecordBuffer, page->getRecordSize(index), index);
    std::shared_ptr<Key> key = kvFactory.getKey();
    return key;
}

std::shared_ptr<Value> BTNode::getValue(uint8 index) {
    assert(index <= page->getRecordCount());
    page->readRecord(temporaryRecordBuffer, page->getRecordSize(index), index);
    std::shared_ptr<Value> value = kvFactory.getValue();
    return value;
}

uint8 BTNode::getItemCount() {
    return page->getRecordCount();
}

uint16 BTNode::serializeToTemporaryBuffer(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint8 keySize = key->size();
    uint8 keyType = key->getKeyType();
    uint8 valueType = value->getType();
    uint8 valueSize = value->size();
    uint16 totalSize = keySize + valueSize + 4;
    char *p = temporaryRecordBuffer;
    memcpy(p, &keyType, sizeof(keyType));
    p += sizeof(keyType);
    memcpy(p, &keySize, sizeof(keySize));
    p += sizeof(keySize);
    key->serialize(p);
    p += keySize;
    memcpy(p, &valueType, sizeof(valueType));
    p += sizeof(valueType);
    memcpy(p, &valueSize, sizeof(valueSize));
    p += sizeof(valueSize);
    value->serialize(p);
    return totalSize;
}