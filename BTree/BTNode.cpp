#include "BTNode.h"
#include "NoSpaceException.h"
#include<iostream>
#include <cassert>
#include<string.h>

BTNode::BTNode(std::shared_ptr<Page> page) {
    this->page = page;
    temporaryRecordBuffer = new char[RECORD_MAX_SIZE];
    kvFactory = KeyValueFactory(temporaryRecordBuffer);
}

void BTNode::insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint16 count = page->getRecordCount();

    if(count == 0) {
        uint16 totalSize = serializeToTemporaryBuffer(key, value);
        try {
            page->insertRecord(temporaryRecordBuffer, totalSize);
        } catch (NoSpaceException error) {
            throw error;
        }
        return;
    }
    for(uint16 i = 0; i < count; i++) {
        page->readRecord(temporaryRecordBuffer, page->getRecordSize(i), i);
        std::shared_ptr<Key> _key = kvFactory.getKey();
        if(key->isEqual(_key)) {
            assert(false);
        } else if(key->compare(_key)) {
            uint16 totalSize = serializeToTemporaryBuffer(key, value);
            try {
                page->insertRecord(temporaryRecordBuffer, totalSize, i);
            } catch (NoSpaceException error) {
                throw error;
            }
            return;
        }
    }
    uint16 totalSize = serializeToTemporaryBuffer(key, value);
    page->insertRecord(temporaryRecordBuffer, totalSize, count);
}

void BTNode::remove(uint16 index) {
    assert(index < page->getRecordCount());
    page->removeRecord(index);
}


std::shared_ptr<Key> BTNode::getKey(uint16 index) {
    assert(index < page->getRecordCount());
    page->readRecord(temporaryRecordBuffer, page->getRecordSize(index), index);
    std::shared_ptr<Key> key = kvFactory.getKey();
    return key;
}

std::shared_ptr<Value> BTNode::getValue(uint16 index) {
    assert(index < page->getRecordCount());
    page->readRecord(temporaryRecordBuffer, page->getRecordSize(index), index);
    std::shared_ptr<Value> value = kvFactory.getValue();
    return value;
}

uint16 BTNode::getItemCount() {
    return page->getRecordCount();
}

uint16 BTNode::serializeToTemporaryBuffer(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint8 keySize = key->size();
    uint8 keyType = key->getKeyType();
    uint8 valueType = value->getType();
    uint8 valueSize = value->size();
    uint16 totalSize = keySize + valueSize + 4 + 16;
    uint64 childPointer = 0;
    char *p = temporaryRecordBuffer;
    memcpy(p, &childPointer, sizeof(childPointer));
    p += sizeof(childPointer);
    memcpy(p, &childPointer, sizeof(childPointer));
    p += sizeof(childPointer);
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

void BTNode::split(std::shared_ptr<BTNode> splittedNode) {
    uint16 count = page->getRecordCount();
    uint16 start = count/2;
    std::shared_ptr<Page> splittedPage = splittedNode->page;
    for(uint16 i = start; i < count; i++) {
        uint16 recordSize = page->getRecordSize(i);
        page->readRecord(temporaryRecordBuffer, recordSize, i);
        splittedPage->insertRecord(temporaryRecordBuffer, recordSize, i - start);
    }

    for(uint16 i = start; i < count; i++) {
        page->removeRecord(start);
    }
}

uint64 BTNode::getChildID(uint16 index) {
    uint16 recordCount = page->getRecordCount();
    assert(index <= recordCount);
    uint16 accessIndex = index == recordCount ? index - 1 : index;
    page->readRecord(temporaryRecordBuffer, page->getRecordSize(accessIndex), accessIndex);
    uint64 *childPointer = (uint64*)(index == recordCount ? (temporaryRecordBuffer + sizeof(uint64)) : temporaryRecordBuffer);
    return *childPointer;
}

void BTNode::setChildID(uint16 index, uint64 id) {
    uint16 recordCount = page->getRecordCount();
    assert(index <= recordCount);
    uint16 accessIndex = index == recordCount ? index - 1 : index;
    page->readRecord(temporaryRecordBuffer, page->getRecordSize(accessIndex), accessIndex);
    uint64 *childPointer = (uint64*)(index == recordCount ? (temporaryRecordBuffer + sizeof(uint64)) : temporaryRecordBuffer);
    *childPointer = id;
    page->updateRecord(temporaryRecordBuffer, accessIndex);
}

bool BTNode::isRootNode() {
    return page->getPageType() == BT_ROOT_PAGE;
}

bool BTNode::isLeafNode() {
    return page->getPageType() == BT_LEAF_PAGE;
}

uint16 BTNode::search(std::shared_ptr<Key> key) {
    uint16 count = page->getRecordCount();
    for(uint16 i = 0; i < count; i++) {
        page->readRecord(temporaryRecordBuffer, page->getRecordSize(i), i);
        std::shared_ptr<Key> _key = kvFactory.getKey();
        if(key->isEqual(key)) {
            return i;
        }
    }
    return count;
}

uint16 BTNode::searchCmp(std::shared_ptr<Key> key) {
    uint16 count = page->getRecordCount();
    for(uint16 i = 0; i < count; i++) {
        page->readRecord(temporaryRecordBuffer, page->getRecordSize(i), i);
        std::shared_ptr<Key> _key = kvFactory.getKey();
        if(!_key->compare(key)) {
            return i;
        }
    }
    return count;
}

void BTNode::swapID(std::shared_ptr<BTNode> node) {
    uint64 id = node->page->id;
    node->page->id = page->id;
    page->id = id;
    page->_isDirty = node->page->_isDirty = true;
}

void BTNode::setRootNode() {
    page->setPageType(BT_ROOT_PAGE);
}

void BTNode::setLeafNode() {
    page->setPageType(BT_LEAF_PAGE);
}

uint64 BTNode::getID() {
    return page->getID();
}

bool BTNode::isInternalNode() {
    return page->getPageType() == BT_INTERNAL_PAGE;
}