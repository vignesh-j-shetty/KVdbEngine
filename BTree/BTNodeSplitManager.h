#ifndef BTNODE_SPLIT_MANAGER
#define BTNODE_SPLIT_MANAGER
#include <memory>
#include <stack>
#include "BTNode.h"
#include "BTNodeBufferPoolManager.h"
class BTNodeSplitManager {
    public:
    BTNodeSplitManager() {
        
    }
    BTNodeSplitManager(std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager);
    void handleSplit(std::shared_ptr<BTNode> node, std::stack<uint64> &nodeStack, std::shared_ptr<Key> key, std::shared_ptr<Value> value);
    void handleRootSplit(std::shared_ptr<BTNode> root, std::shared_ptr<Key> key, std::shared_ptr<Value> value);
    private:
    std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager;
    std::shared_ptr<Value> emptyValue;
};
#endif