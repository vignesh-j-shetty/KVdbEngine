#include "BTree.h"
#include "BTNode.h"
#include "BTNodeBufferPoolManager.h"
void BTree::insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    BTNode rootNode = bufferPoolManager.getRootPage();
    try {
        rootNode.insert(key, value);
    } catch(NoSpaceException error) {
        
    }
}