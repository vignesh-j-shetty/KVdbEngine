#include "BTree.h"
#include "BTNode.h"
#include "BTNodeBufferPoolManager.h"
#include "BufferPoolManager.h"
#include<iostream>
#include <queue>
#include <cassert>


BTree::BTree() : diskManager(new DiskManager("dd.db")) {
    emptyValue = std::shared_ptr<Value>(new StringValue(""));
    std::shared_ptr<BufferPoolManager> _bufferPoolManager(new BufferPoolManager(diskManager));
    bufferPoolManager = std::shared_ptr<BTNodeBufferPoolManager>(new BTNodeBufferPoolManager(_bufferPoolManager));
    splitManager = BTNodeSplitManager(bufferPoolManager);
    deletionManager = BTDeletionManager(bufferPoolManager);
}

void BTree::insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    std::shared_ptr<BTNode> rootNode = bufferPoolManager->getRootPage();
    if(rootNode->getItemCount() == 0) {
        rootNode->setNodeType(LEAF_NODE);
    }
    std::stack<uint64> nodeStack;
    std::shared_ptr<BTNode> currentNode = searchNode(key, nodeStack);

    try {
        currentNode->insert(key, value);
    } catch(NoSpaceException e) {
        if(nodeStack.empty()) {
            splitManager.handleRootSplit(currentNode, key, value);
        } else {
            splitManager.handleSplit(currentNode, nodeStack, key, value);
        }
    }
}

std::shared_ptr<BTNode> BTree::searchNode(std::shared_ptr<Key> key, std::stack<uint64> &nodeStack) {
    std::shared_ptr<BTNode> currentNode = bufferPoolManager->getRootPage();
    while (!(currentNode->getNodeType() == LEAF_NODE)) {
        nodeStack.push(currentNode->getID());
        uint16 index = currentNode->searchCmp(key);
        uint64 id = currentNode->getChildID(index);
        assert(id != 0);
        currentNode = bufferPoolManager->getNode(id);
        assert(currentNode->getItemCount() != 0);
    }
    return currentNode;
}

bool BTree::isKeyPresent(std::shared_ptr<Key> key) {
    std::stack<uint64> nodeStack;
    std::shared_ptr<BTNode> currentNode = searchNode(key, nodeStack);
    uint16 index = currentNode->search(key);
    return index == currentNode->getItemCount() ? false : true;
}

void BTree::remove(std::shared_ptr<Key> key) {
    std::stack<uint64> nodeStack;
    std::shared_ptr<BTNode> currentNode = bufferPoolManager->getRootPage();
    uint64 nonLeafNodeID = 0;
    while (!(currentNode->getNodeType() == LEAF_NODE)) {
        nodeStack.push(currentNode->getID());
        uint16 index = currentNode->searchCmp(key);
        uint16 leafNodeIndex = currentNode->search(key);
        if(leafNodeIndex < currentNode->getItemCount()) {
            nonLeafNodeID = currentNode->getID();
        }
        uint64 id = currentNode->getChildID(index);
        assert(id != 0);
        currentNode = bufferPoolManager->getNode(id);
        assert(currentNode->getItemCount() != 0);
    }
    uint16 index = currentNode->search(key);
    assert(index != currentNode->getItemCount());
    currentNode->remove(index);
    if(currentNode->isRootNode()) {
        return;
    }
    std::shared_ptr<BTNode> keyDeletedNode = currentNode;
    if(currentNode->getItemCount() == 0) {
        do {
            uint64 parentID = nodeStack.top();
            nodeStack.pop();
            std::shared_ptr<BTNode> parentNode = bufferPoolManager->getNode(parentID);
            uint16 childIndexInParent = parentNode->searchChild(currentNode->getID());
            if(parentNode->getItemCount() == 0 || !deletionManager.borrow(currentNode, parentNode)) {
                BTNodeMergeType mergeType = deletionManager.merge(currentNode, parentNode);
                if(mergeType == LEFT_MERGE) {
                    parentNode->setChildID(childIndexInParent - 1, parentNode->getChildID(childIndexInParent));
                    parentNode->remove(childIndexInParent - 1);
                } else if(mergeType == RIGHT_MERGE) {
                    parentNode->setChildID(childIndexInParent + 1, parentNode->getChildID(childIndexInParent));
                    parentNode->remove(childIndexInParent);
                }
                if(nonLeafNodeID != 0) {
                    std::shared_ptr<BTNode> nonLeafNode = bufferPoolManager->getNode(nonLeafNodeID);
                    uint16 _index = nonLeafNode->search(key);
                    if(_index < nonLeafNode->getItemCount()) {
                        std::shared_ptr<Key> _key = keyDeletedNode->getKey(0);
                        nonLeafNode->updateKeyValue(_key, emptyValue, _index);
                    }
                }
            }
            currentNode = parentNode;
        } while(currentNode->getItemCount() == 0);
    }
}

std::shared_ptr<Value> BTree::getValue(std::shared_ptr<Key> key) {
    std::stack<uint64> nodeStack;
    std::shared_ptr<BTNode> currentNode = searchNode(key, nodeStack);
    uint16 index = currentNode->search(key);
    assert(index != currentNode->getItemCount());
    return currentNode->getValue(index);
}