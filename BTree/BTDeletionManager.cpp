#include "BTDeletionManager.h"

BTDeletionManager::BTDeletionManager(std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager) {
    this->bufferPoolManager = bufferPoolManager;
    emptyValue = std::shared_ptr<Value>(new StringValue(""));
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
    std::shared_ptr<Key> rightSiblingtMinKey = rightSibling->getKey(0);
    std::shared_ptr<Value> rightSiblingtValue= rightSibling->getValue(0);
    node->insert(rightSiblingtMinKey, rightSiblingtValue);
    rightSibling->remove(0);
    std::shared_ptr<Key> updateKey = rightSibling->getKey(0);
    parentNode->updateKeyValue(updateKey, emptyValue, indexInParentNode);
}

void BTDeletionManager::leftBorrow(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode, std::shared_ptr<BTNode> leftSibling) {
    uint16 indexInParentNode = parentNode->searchChild(node->getID());
    uint16 leftSiblingSize = leftSibling->getItemCount();
    std::shared_ptr<Key> leftSiblingtMaxKey = leftSibling->getKey(leftSiblingSize - 1);
    std::shared_ptr<Value> leftSiblingValue= leftSibling->getValue(leftSiblingSize - 1);
    node->insert(leftSiblingtMaxKey, leftSiblingValue);
    leftSibling->remove(leftSiblingSize - 1);
    parentNode->updateKeyValue(leftSiblingtMaxKey, emptyValue, indexInParentNode - 1);
}

BTNodeMergeType BTDeletionManager::merge(std::shared_ptr<BTNode> node, std::shared_ptr<BTNode> parentNode) {
    uint16 indexInParentNode = parentNode->searchChild(node->getID());
    assert(indexInParentNode != parentNode->getItemCount() + 1);
    if(indexInParentNode == 0 || indexInParentNode == parentNode->getItemCount()) {
        if(indexInParentNode == 0) {
            std::shared_ptr<BTNode> rightSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode + 1));
            assert(!rightSibling->hasMinimum());
            node->merge(rightSibling);
            return RIGHT_MERGE;
        } else {
            std::shared_ptr<BTNode> leftSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode - 1));
            assert(!leftSibling->hasMinimum());
            node->merge(leftSibling);
            return LEFT_MERGE;
        }
    } else {
        std::shared_ptr<BTNode> rightSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode + 1));
        std::shared_ptr<BTNode> leftSibling = bufferPoolManager->getNode(parentNode->getChildID(indexInParentNode - 1));
        if(!rightSibling->hasMinimum()) {
            node->merge(rightSibling);
            return RIGHT_MERGE;
        } else if(!leftSibling->hasMinimum()) {
            node->merge(leftSibling);
            return LEFT_MERGE;
        }
    }
}