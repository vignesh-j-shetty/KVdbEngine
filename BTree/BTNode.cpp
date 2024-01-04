#include "BTNode.h"
#include<iostream>
#include <cassert>
#include<string.h>
#include "KeyValueFactory.h"
BTNode::BTNode(std::shared_ptr<Page> page) {
    this->page = page;
    this->temporaryRecordBuffer = new char[RECORD_MAX_SIZE];
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
        std::shared_ptr<Key> _key = KeyValueFactory::getKeyFromSerilizedData(temporaryRecordBuffer);
        if(key->compare(_key)) {
            uint16 totalSize = serializeToTemporaryBuffer(key, value);
            page->insertRecord(temporaryRecordBuffer, totalSize, i);
            return;
        }
    }
    uint16 totalSize = serializeToTemporaryBuffer(key, value);
    page->insertRecord(temporaryRecordBuffer, totalSize, count);
}

//Temporary function later to be removed
void BTNode::printKeys() {
    uint8 count = page->getRecordCount();
    for (uint8 i = 0; i < count; i++) {
        page->readRecord(temporaryRecordBuffer, page->getRecordSize(i), i);
        std::shared_ptr<Key> _key = KeyValueFactory::getKeyFromSerilizedData(temporaryRecordBuffer);
        std::cout<<std::any_cast<std::string>(_key->getData())<<std::endl;
    }
}

uint16 BTNode::serializeToTemporaryBuffer(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint8 keySize = key->size();
    uint8 keyType = key->getKeyType();
    uint16 totalSize = keySize + value->size() + 2;
    char *p = temporaryRecordBuffer;
    memcpy(p, &keyType, sizeof(keyType));
    p += sizeof(keyType);
    memcpy(p, &keySize, sizeof(keySize));
    p += sizeof(keySize);
    key->serialize(p);
    p += keySize;
    value->serialize(p);
    return totalSize;
}