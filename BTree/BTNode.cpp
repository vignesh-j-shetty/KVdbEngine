#include "BTNode.h"
#include<iostream>
#include<cassert>
BTNode::BTNode(Page page) {
    this->page = page;
    pageIterator = page.getIterator();
}

void BTNode::insertKeyValue(BTKeyValuePair keyValuePair) {
    pageIterator->reset();
    uint16 size = pageIterator->count();
    uint16 totalSize = keyValuePair.keySize + keyValuePair.valueSize + 5; //5 is total size of meta data
    if(size == 0) {
        char *memoryBlock = page.allocateMemoryBlock(totalSize);
        keyValuePair.serialize(memoryBlock);
        return;
    }
    uint16 index = 0;
    for (uint16 offset = pageIterator->getCurrentItem(); pageIterator->hasNext(); pageIterator->next()) {
        char *memoryBlock = page.getMemory(offset);
        BTKeyValuePair _keyValuePair = BTKeyValuePair::deSerialize(memoryBlock);
        // if keyValuePair < _keyValuePair
        if(keyCompareSm(keyValuePair, _keyValuePair)) {
            char *memoryBlock = page.allocateMemoryBlockAtSlot(totalSize, index);
            keyValuePair.serialize(memoryBlock);
            return;
        }
        index++;
    }
}

bool BTNode::keyCompareSm(BTKeyValuePair keyValuePair1, BTKeyValuePair keyValuePair2) {
    assert(keyValuePair1.keyType == keyValuePair2.keyType);
    switch (keyValuePair1.keyType) {
    case STR:
    return cmp(keyValuePair1.key, keyValuePair2.key);
    default:
    break;
    }
    assert(false && "Invalid key type");
    return false;
}

bool BTNode::cmp(char *str1, char *str2) {
    int d = strcmp(str1, str2);
    return d < 0;
}