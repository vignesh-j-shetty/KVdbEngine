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

void BTree::debugPrint() {
    std::shared_ptr<BTNode> currentNode = bufferPoolManager->getRootPage();
    std::queue<std::shared_ptr<BTNode>> nodeQueue;
    nodeQueue.push(currentNode);
    uint16 lvl = 0;
    while(!nodeQueue.empty()) {
        uint16 nodesInCurrentLevel = nodeQueue.size();
        lvl++;
        for(uint16 i = 0; i < nodesInCurrentLevel; i++) {
             currentNode = nodeQueue.front();
             nodeQueue.pop();
             printNode(currentNode);
             if(!(currentNode->getNodeType() == LEAF_NODE)) {
                uint16 n = currentNode->getItemCount();
                for (uint16 j = 0; j <= n; j++) {
                    uint64 id = currentNode->getChildID(j);
                    assert(id != 0);
                    std::shared_ptr<BTNode> node = bufferPoolManager->getNode(id);
                    nodeQueue.push(node);
                }
            }
        }
        std::cout<<std::endl;
    }
}

void BTree::printNode(std::shared_ptr<BTNode> node) {
    const bool isLeaf = node->getNodeType() == LEAF_NODE;
    if(isLeaf) {
        std::cout<<" || ";
    } else {
        std::cout<<" | ";
    }
    //std::cout<<"id : "<<node->getID()<<"> ";
    uint16 n = node->getItemCount();
    for (uint16 i = 0; i < n; i++) {
        uint64 keyValue = std::any_cast<uint64>(node->getKey(i)->getData());
        std::cout<<keyValue;
        if(i != n - 1) {
            std::cout<<" , ";
        }

    }
    if(isLeaf) {
        std::cout<<" || ";
    } else {
        std::cout<<" | ";
    }
}

void BTree::debugPrintKeyChild(std::shared_ptr<BTNode> node) {
    std::cout<<"ID : "<<node->getID()<<std::endl;
    for(uint16 i = 0; i <= node->getItemCount(); i++) {
        if(i == node->getItemCount()) {
            std::cout<<"Key " <<node->getChildID(i)<<std::endl;
        } else {
            std::cout<<i<<" : "<<std::any_cast<uint64>(node->getKey(i)->getData())<<" "<<node->getChildID(i)<<std::endl;
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
    if(currentNode->getItemCount() > 0 && nonLeafNodeID != 0) {
        std::shared_ptr<BTNode> nonLeafNode = bufferPoolManager->getNode(nonLeafNodeID);
        uint16 _index = nonLeafNode->search(key);
        assert(_index != nonLeafNode->getItemCount());
        std::shared_ptr<Key> _key = currentNode->getKey(0);
        nonLeafNode->updateKeyValue(_key, emptyValue, _index);
    } else if(currentNode->getItemCount() == 0) {
        while (true) {
            uint64 parentID = nodeStack.top();
            nodeStack.pop();
            std::shared_ptr<BTNode> parentNode = bufferPoolManager->getNode(parentID);
            uint16 childIndexInParent = parentNode->searchChild(currentNode->getID());
            if(!deletionManager.borrow(currentNode, parentNode)) {
                if(deletionManager.merge(currentNode, parentNode) == LEFT_MERGE) {
                    parentNode->remove(childIndexInParent - 1);
                } else {
                    parentNode->remove(childIndexInParent);
                }
                if(!parentNode->hasMinimum()) {
                    currentNode = parentNode;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
}