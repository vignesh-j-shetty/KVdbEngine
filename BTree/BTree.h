#ifndef _BTREE_
#define _BTREE_
#include "Key.h"
#include "Value.h"
#include "BTNodeBufferPoolManager.h"
#include "DiskManager.h"
#include "BTNodeSplitManager.h"
#include "BTDeletionManager.h"
#include<memory>
#include <stack>

class BTree {
    public:
    BTree();
    ~BTree() {
        bufferPoolManager->flushAll();
    }
    void insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value);
    void remove(std::shared_ptr<Key> key);
    void debugPrint();
    bool isKeyPresent(std::shared_ptr<Key> key);
    private:
    std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager;
    std::shared_ptr<DiskManager> diskManager;
    std::shared_ptr<Value> emptyValue;
    void printNode(std::shared_ptr<BTNode> rootNode);
    void debugPrintKeyChild(std::shared_ptr<BTNode> node);
    std::shared_ptr<BTNode> searchNode(std::shared_ptr<Key> key, std::stack<uint64> &nodeStack);
    BTNodeSplitManager splitManager;
    BTDeletionManager deletionManager;
};

#endif