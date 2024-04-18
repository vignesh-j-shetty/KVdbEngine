#ifndef BT_DELETION_MANAGER
#define BT_DELETION_MANAGER
#include <memory>
#include "BTNodeBufferPoolManager.h"

enum BTNodeMergeType {
    RIGHT_MERGE, LEFT_MERGE
};

class BTDeletionManager {
    public:
    BTDeletionManager(std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager);
    BTDeletionManager() {

    }
    // Returns true if borrowed from sibling nodes
    bool borrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode);
    BTNodeMergeType merge(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode);
    private:
    std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager;
    std::shared_ptr<Value> emptyValue;
    void rightBorrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode, std::shared_ptr<BTNode> rightSibling);
    void leftBorrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode, std::shared_ptr<BTNode> leftSibling);
};
#endif