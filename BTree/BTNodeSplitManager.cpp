#include "BTNodeSplitManager.h"


BTNodeSplitManager::BTNodeSplitManager(std::shared_ptr<BTNodeBufferPoolManager> bufferPoolManager) {
    this->bufferPoolManager = bufferPoolManager;
    emptyValue = std::shared_ptr<Value>(new StringValue(""));
}

void BTNodeSplitManager::handleSplit(std::shared_ptr<BTNode> node, std::stack<uint64> &nodeStack, std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint64 previousLeftChild = 0;
    uint64 previousRightChild = 0;
    uint64 debug_iter_counter = 0;
    while (true) {
        debug_iter_counter++;
        try {
            uint16 index = node->insert(key, value);
            node->setChildID(index, previousLeftChild);
            node->setChildID(index + 1, previousRightChild);
            break;
        } catch(NoSpaceException e) {
            bufferPoolManager->setIsPinnnedStatus(node->getID(), true);
            std::shared_ptr<BTNode> splittedNode = bufferPoolManager->newNode();
            bufferPoolManager->setIsPinnnedStatus(splittedNode->getID(), true);
            uint16 insertedIndex = node->searchCmp(key);
            uint16 countBeforeSplit = node->getItemCount();
            node->split(splittedNode);
            uint16 midIndex = countBeforeSplit/2;
            if(insertedIndex != midIndex) {
                std::shared_ptr<BTNode> nodeToInserted = insertedIndex < midIndex ? node : splittedNode;
                uint16 index = nodeToInserted->insert(key, value);
                nodeToInserted->setChildID(index, previousLeftChild);
                nodeToInserted->setChildID(index + 1, previousRightChild);
                key = splittedNode->getKey(0);
                if(splittedNode->getNodeType() == INTERNAL_NODE) {
                    splittedNode->remove(0);
                }
            } else {
                node->setChildID(node->getItemCount(), previousLeftChild);
                splittedNode->setChildID(0, previousRightChild);
                if(node->getNodeType() == LEAF_NODE) {
                    splittedNode->insert(key, value);
                }
            }
            value = emptyValue;
            previousLeftChild = node->getID();
            previousRightChild = splittedNode->getID();
            if(node->getNodeType() == ROOT_NODE) {
                std::shared_ptr<BTNode> newRootNode = bufferPoolManager->newNode();
                newRootNode->swapID(node);
                newRootNode->insert(key, emptyValue);
                newRootNode->setChildID(0, node->getID());
                newRootNode->setChildID(1, splittedNode->getID());
                splittedNode->remove(0);
                node->setNodeType(INTERNAL_NODE);
                splittedNode->setNodeType(INTERNAL_NODE);
                bufferPoolManager->setIsPinnnedStatus(node->getID(), false);
                bufferPoolManager->setIsPinnnedStatus(splittedNode->getID(), false);
                break;
            }
            bufferPoolManager->setIsPinnnedStatus(node->getID(), false);
            bufferPoolManager->setIsPinnnedStatus(splittedNode->getID(), false);
            if(!nodeStack.empty()) {
                node = bufferPoolManager->getNode(nodeStack.top());
                nodeStack.pop();
            }
        }
    }
}

void BTNodeSplitManager::handleRootSplit(std::shared_ptr<BTNode> root, std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    std::shared_ptr<BTNode> splittedNode = bufferPoolManager->newNode();
    std::shared_ptr<BTNode> rootNode = bufferPoolManager->newNode();
    std::shared_ptr<BTNode> node = root;
    uint16 insertIndex = node->searchCmp(key);
    uint16 countBeforeSplit = node->getItemCount();
    uint16 midIndex = countBeforeSplit/2;
    node->swapID(rootNode);
    node->split(splittedNode);
    if(insertIndex <= midIndex) {
        node->insert(key, value);
    } else {
        splittedNode->insert(key, value);
    }
    std::shared_ptr<Key> medianKey = splittedNode->getKey(0);
    node->setNodeType(LEAF_NODE);;
    splittedNode->setNodeType(LEAF_NODE);;
    rootNode->insert(medianKey, emptyValue);
    rootNode->setChildID(0, node->getID());
    rootNode->setChildID(1, splittedNode->getID());
    rootNode->setNodeType(ROOT_NODE);
}