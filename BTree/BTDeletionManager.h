#ifndef BT_DELETION_MANAGER
#define BT_DELETION_MANAGER
#include <memory>
#include "BTNodeBufferPoolManager.h"

class BTDeletionManager {
    public:
    BTDeletionManager(std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager);
    BTDeletionManager() {
        
    }
    void borrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode);
    private:
    std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager;
    std::shared_ptr<Value> emptyValue;
    void rightBorrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode, std::shared_ptr<BTNode> rightSibling);
    void leftBorrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode, std::shared_ptr<BTNode> leftSibling);
};
#endif