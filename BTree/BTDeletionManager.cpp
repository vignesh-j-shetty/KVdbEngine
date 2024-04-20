#include "BTDeletionManager.h"
BTDeletionManager::BTDeletionManager(std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager) {
    this->bufferPoolManager = bufferPoolManager;
    emptyValue = std::shared_ptr<Value>(new StringValue(""));
    prevMergedChild = 0;
}

bool BTDeletionManager::borrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode) {
    uint16 indexInParentNode = parentNode->searchChild(node->getID());
    assert(indexInParentNode != parentNode->getItemCount() + 1);
    if(indexInParentNode == 0 || indexInParentNode == parentNode->getItemCount()) {
        if(indexInParentNode == 0) {
            std::shared_ptr<BTNode> rightSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode + 1));
            if(rightSibling->hasMinimum()) {
                rightBorrow(node, parentNode, rightSibling);
                return true;
            }
        } else {
            std::shared_ptr<BTNode> leftSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode - 1));
            if(leftSibling->hasMinimum()) {
                leftBorrow(node, parentNode, leftSibling);
                return true;
            }
        }
    } else {
        std::shared_ptr<BTNode> rightSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode + 1));
        std::shared_ptr<BTNode> leftSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode - 1));
        if(rightSibling->hasMinimum()) {
            rightBorrow(node, parentNode, rightSibling);
            return true;
        } else if(leftSibling->hasMinimum()) {
            leftBorrow(node, parentNode, leftSibling);
            return true;
        }
    }
    return false;
}

void BTDeletionManager::rightBorrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode, std::shared_ptr<BTNode> rightSibling) {
    uint16 indexInParentNode = parentNode->searchChild(node->getID());
    if(node->getNodeType() == LEAF_NODE) {
        std::shared_ptr<Key> rightSiblingtMinKey = rightSibling->getKey(0);
        std::shared_ptr<Value> rightSiblingtValue= rightSibling->getValue(0);
        node->insert(rightSiblingtMinKey, rightSiblingtValue);
        rightSibling->remove(0);
        std::shared_ptr<Key> updateKey = rightSibling->getKey(0);
        parentNode->updateKeyValue(updateKey, emptyValue, indexInParentNode);
    } else {
        std::shared_ptr<Key> updateKey = rightSibling->getKey(0);
        std::shared_ptr<Key> parentKey = parentNode->getKey(indexInParentNode);
        uint16 insertIndex = node->insert(parentKey, emptyValue);
        node->setChildID(insertIndex + 1, rightSibling->getChildID(0));
        node->setChildID(insertIndex, prevMergedChild);
        rightSibling->remove(0);
        parentNode->updateKeyValue(updateKey, emptyValue, indexInParentNode);
    }
}

void BTDeletionManager::leftBorrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode, std::shared_ptr<BTNode> leftSibling) {
    uint16 indexInParentNode = parentNode->searchChild(node->getID());
    uint16 leftSiblingSize = leftSibling->getItemCount();
    if(node->getNodeType() == LEAF_NODE) {
        std::shared_ptr<Key> leftSiblingtMaxKey = leftSibling->getKey(leftSiblingSize - 1);
        std::shared_ptr<Value> leftSiblingValue= leftSibling->getValue(leftSiblingSize - 1);
        node->insert(leftSiblingtMaxKey, leftSiblingValue);
        leftSibling->remove(leftSiblingSize - 1);
        parentNode->updateKeyValue(leftSiblingtMaxKey, emptyValue, indexInParentNode - 1);
    } else {
        std::shared_ptr<Key> updateKey = leftSibling->getKey(leftSiblingSize - 1);
        std::shared_ptr<Key> parentKey = parentNode->getKey(indexInParentNode - 1);
        uint16 insertIndex = node->insert(parentKey, emptyValue);
        node->setChildID(insertIndex + 1, prevMergedChild);
        node->setChildID(insertIndex, leftSibling->getChildID(leftSiblingSize));
        leftSibling->remove(leftSiblingSize - 1);
        parentNode->updateKeyValue(updateKey, emptyValue, indexInParentNode - 1);
    }
}

BTNodeMergeType BTDeletionManager::merge(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode) {
    uint16 indexInParentNode = parentNode->searchChild(node->getID());
    assert(indexInParentNode != parentNode->getItemCount() + 1);
    if(node->getNodeType() == LEAF_NODE && parentNode->isRootNode() && parentNode->getItemCount() == 1) {
         uint16 parentNodeSize = parentNode->getItemCount();
         for(uint16 i = 0; i <= parentNodeSize; i++) {
            uint64 childID = parentNode->getChildID(i);
            if(childID != node->getID()) {
                node->merge(bufferPoolManager->getNode(childID));
            }
         }
         node->swapID(parentNode);
         return INTERNAL_NODE_MERGE;
    }
    if(node->getNodeType() == LEAF_NODE) {
        if(indexInParentNode == 0 || indexInParentNode == parentNode->getItemCount()) {
            if(indexInParentNode == 0) {
                std::shared_ptr<BTNode> rightSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode + 1));
                assert(!rightSibling->hasMinimum());
                node->merge(rightSibling);
                bufferPoolManager->deleteNode(rightSibling->getID());
                prevMergedChild = node->getID();
                return RIGHT_MERGE;
            } else {
                std::shared_ptr<BTNode> leftSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode - 1));
                assert(!leftSibling->hasMinimum());
                node->merge(leftSibling);
                bufferPoolManager->deleteNode(leftSibling->getID());
                prevMergedChild = node->getID();
                return LEFT_MERGE;
            }
        } else {
            std::shared_ptr<BTNode> rightSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode + 1));
            std::shared_ptr<BTNode> leftSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode - 1));
            if(!rightSibling->hasMinimum()) {
                node->merge(rightSibling);
                bufferPoolManager->deleteNode(rightSibling->getID());
                prevMergedChild = node->getID();
                return RIGHT_MERGE;
            } else if(!leftSibling->hasMinimum()) {
                node->merge(leftSibling);
                bufferPoolManager->deleteNode(leftSibling->getID());
                prevMergedChild = node->getID();
                return LEFT_MERGE;
            }
        }
    } else {
        uint16 parentNodeSize = parentNode->getItemCount();
        for(uint16 i = 0; i <= parentNodeSize; i++) {
            uint64 childid = parentNode->getChildID(i);
            std::shared_ptr<BTNode> childNode = bufferPoolManager->getNode(childid);
            if(childNode->getItemCount() == 0) {
                parentNode->setChildID(i, prevMergedChild);
            } else {
                parentNode->merge(childNode);
            }
        }
        prevMergedChild = node->getID();
        return INTERNAL_NODE_MERGE;
    }
    assert(false && "Should have reached end of function");
    return LEFT_MERGE;
}

void BTDeletionManager::setPrevMergedNodeNull() {
    prevMergedChild = 0;
}