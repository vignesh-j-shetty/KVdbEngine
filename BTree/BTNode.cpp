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

uint16 BTNode::insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint16 count = page->getRecordCount();
    uint64 lastChildID = getChildID(count);
    if(count == 0) {
        uint16 totalSize = serializeToTemporaryBuffer(key, value);
        try {
            page->insertRecord(temporaryRecordBuffer, totalSize);
        } catch (NoSpaceException error) {
            throw error;
        }
        return 0;
    }
    for(uint16 i = 0; i < count; i++) {
        page->readRecord(temporaryRecordBuffer, page->getRecordSize(i), i);
        std::shared_ptr<Key> _key = kvFactory.getKey();
        if(*key == _key) {
            assert(false);
        } else if(*key < _key) {
            uint16 totalSize = serializeToTemporaryBuffer(key, value);
            try {
                page->insertRecord(temporaryRecordBuffer, totalSize, i);
                return i;
            } catch (NoSpaceException error) {
                throw error;
            }
        }
    }
    // When greater than all elements in node
    uint16 totalSize = serializeToTemporaryBuffer(key, value);
    page->insertRecord(temporaryRecordBuffer, totalSize, count);
    setChildID(count, lastChildID);
    return count;
}

void BTNode::remove(uint16 index) {
    uint16 count = page->getRecordCount();
    assert(index < count);
    if(index == (count - 1) && index > 0) {
        uint64 lastChildIdAfterDelete = getChildID(index);
        uint64* id = (uint64*) temporaryRecordBuffer;
        *id = lastChildIdAfterDelete;
        page->updateOtherData(temporaryRecordBuffer);
    }
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
    // 4 is keySize, KeyType, valueType and valueSize and 8 is childPointer
    uint16 totalSize = keySize + valueSize + 4 + 8;
    uint64 childPointer = 0;
    char *p = temporaryRecordBuffer;
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
    splittedNode->page->setPageType(page->getPageType());
    uint16 count = page->getRecordCount();
    uint16 start = count/2;
    std::shared_ptr<Page> splittedPage = splittedNode->page;

    for(uint16 i = start; i < count; i++) {
        uint16 recordSize = page->getRecordSize(i);
        page->readRecord(temporaryRecordBuffer, recordSize, i);
        splittedPage->insertRecord(temporaryRecordBuffer, recordSize, i - start);
    }
    uint64 lastChildIDAfterDelete = getChildID(start);
    uint64 lastChild = getChildID(count);
    for(uint16 i = start; i < count; i++) {
        page->removeRecord(start);
    }
    page->readOtherData(temporaryRecordBuffer);
    {
        uint64 *id = (uint64*) temporaryRecordBuffer;
        *id = lastChildIDAfterDelete;
        page->updateOtherData(temporaryRecordBuffer);
    }
    splittedNode->setChildID(splittedNode->getItemCount(), lastChild);
}

uint64 BTNode::getChildID(uint16 index) {
    uint16 recordCount = page->getRecordCount();
    assert(index <= recordCount);
    if(index < recordCount) {
        page->readRecord(temporaryRecordBuffer, sizeof(uint64), index);
    } else {
        page->readOtherData(temporaryRecordBuffer);
    }
    uint64 *ID = (uint64*) temporaryRecordBuffer;
    return *ID;
}

void BTNode::setChildID(uint16 index, uint64 id) {
    uint16 recordCount = page->getRecordCount();
    assert(index <= recordCount);
    uint64 *setID = (uint64*) temporaryRecordBuffer;
    if(index < recordCount) {
        uint16 recordSize = page->getRecordSize(index);
        page->readRecord(temporaryRecordBuffer, recordSize, index);
        *setID = id;
        page->updateRecord(temporaryRecordBuffer, index);
    } else {
        page->readOtherData(temporaryRecordBuffer);
        *setID = id;
        page->updateOtherData(temporaryRecordBuffer);
    }
}

uint16 BTNode::search(std::shared_ptr<Key> key) {
    uint16 count = page->getRecordCount();
    for(uint16 i = 0; i < count; i++) {
        page->readRecord(temporaryRecordBuffer, page->getRecordSize(i), i);
        std::shared_ptr<Key> _key = kvFactory.getKey();
        if(*key == _key) {
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
        if(!(*_key <= key)) {
            return i;
        }
    }
    return count;
}

void BTNode::swapID(std::shared_ptr<BTNode> node) {
    uint64 id = node->page->id;
    node->page->id = page->id;
    page->id = id;
    page->_isDirty = true;
    node->page->_isDirty = true;
}

uint64 BTNode::getID() {
    return page->getID();
}

NodeType BTNode::getNodeType() {
    return mapToNodeType(page->getPageType());
}

void BTNode::setNodeType(NodeType type) {
    page->setPageType(mapToPageType(type));
}

NodeType BTNode::mapToNodeType(PageType type) {    
    switch (type) {
        case BT_ROOT_PAGE:
            return ROOT_NODE;
        case BT_INTERNAL_PAGE:
            return INTERNAL_NODE;
        case BT_LEAF_PAGE:
            return LEAF_NODE;
        case _OVERFLOW:
            return OVERFLOW_NODE;
        default:
        assert(false && "Unknown PageType");
    }
}

PageType BTNode::mapToPageType(NodeType type) {
    switch (type) {
        case ROOT_NODE:
            return BT_ROOT_PAGE;
        case INTERNAL_NODE:
            return BT_INTERNAL_PAGE;
        case LEAF_NODE:
            return BT_LEAF_PAGE;
        case OVERFLOW_NODE:
            return _OVERFLOW;
        default:
            assert(false && "Unknown NodeType");
    }
}

void BTNode::updateKeyValue(std::shared_ptr<Key> key, std::shared_ptr<Value> value, uint16 index) {
    uint64 childID = getChildID(index);
    page->removeRecord(index);
    uint16 totalSize = serializeToTemporaryBuffer(key, value);
    page->insertRecord(temporaryRecordBuffer, totalSize, index);
    setChildID(index, childID);
}

bool BTNode::hasMinimum() {
    return getItemCount() > 1;
}

uint16 BTNode::searchChild(uint64 childID) {
    uint16 n = getItemCount();
    for(uint16 i = 0; i <= n; i++) {
        if(getChildID(i) == childID) {
            return i;
        }
    }
    return n + 1;
}

void BTNode::merge(std::shared_ptr<BTNode> node) {
    uint16 n = node->getItemCount();
    for(uint16 i = 0; i < n; i++) {
        std::shared_ptr<Key> key = node->getKey(i);
        std::shared_ptr<Value> value = node->getValue(i);
        uint16 insertedIndex = insert(key, value);
        setChildID(insertedIndex, node->getChildID(i));
    }
    setChildID(getItemCount(), node->getChildID(n));
}