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
}

void BTree::insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    std::shared_ptr<BTNode> rootNode = bufferPoolManager->getRootPage();
    if(rootNode->getItemCount() == 0) {
        rootNode->setLeafNode();
    }

    // Search Code
    std::shared_ptr<BTNode> currentNode = bufferPoolManager->getRootPage();
    std::stack<std::shared_ptr<BTNode>> nodeStack;
    while (!currentNode->isLeafNode()) {
        nodeStack.push(currentNode);
        uint16 index = currentNode->searchCmp(key);
        uint16 id = currentNode->getChildID(index);
        assert(id != 0);
        currentNode = bufferPoolManager->getNode(id);
    }
    try {
        currentNode->insert(key, value);
    } catch(NoSpaceException e) {
        if(nodeStack.empty()) {
            handleRootSplit(currentNode, key, value);
        } else {
            handleSplit(currentNode, nodeStack, key, value);
        }
    }
}

void BTree::debugPrint() {
    std::shared_ptr<BTNode> currentNode = bufferPoolManager->getRootPage();
    std::queue<std::shared_ptr<BTNode>> nodeQueue;
    nodeQueue.push(currentNode);
    while(!nodeQueue.empty()) {
        uint16 nodesInCurrentLevel = nodeQueue.size();
        for(uint16 i = 0; i < nodesInCurrentLevel; i++) {
             std::shared_ptr<BTNode> currentNode = nodeQueue.front();
             nodeQueue.pop();
             printNode(currentNode);
             if(!currentNode->isLeafNode()) {
                for(uint16 i = 0; i <= currentNode->getItemCount(); i++) {
                    uint64 id = currentNode->getChildID(i);
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
    std::cout<<" | ";
    for (uint16 i = 0; i < node->getItemCount(); i++) {
        std::cout<<std::any_cast<std::string>(node->getKey(i)->getData())<<" - ";
    }
    std::cout<<" | ";
}

void BTree::handleSplit(std::shared_ptr<BTNode> node, std::stack<std::shared_ptr<BTNode>> &nodeStack, std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
    uint64 previousLeftChild = 0;
    uint64 previousRightChild = 0;

    while (true) {
        try {
            uint16 index = node->insert(key, value);
            node->setChildID(index, previousLeftChild);
            node->setChildID(index + 1, previousRightChild);
            break;
        } catch(NoSpaceException e) {
            std::shared_ptr<BTNode> splittedNode = bufferPoolManager->newNode();
            uint16 insertedIndex = node->searchCmp(key);
            uint16 countBeforeSplit = node->getItemCount();
            node->split(splittedNode);
            node->compactSpace();

            uint16 midIndex = countBeforeSplit/2;
            if(insertedIndex < midIndex) {
                uint16 index = node->insert(key, value);
                node->setChildID(index, previousLeftChild);
                node->setChildID(index + 1, previousRightChild);
                key = node->getKey(node->getItemCount() - 1);
            } else if(insertedIndex > midIndex) {
                uint16 index = splittedNode->insert(key,value);
                splittedNode->setChildID(index, previousLeftChild);
                splittedNode->setChildID(index + 1, previousRightChild);
                key = node->getKey(node->getItemCount() - 1);
            } else if(insertedIndex == midIndex) {
                if(splittedNode->isLeafNode()) {
                    node->insert(key, value);
                }
            }
            value = emptyValue;
            previousLeftChild = node->getID();
            previousRightChild = splittedNode->getID();

            if(node->isRootNode()) {
                std::shared_ptr<BTNode> newRootNode = bufferPoolManager->newNode();
                std::shared_ptr<Key> key = node->getKey(node->getItemCount() - 1);
                newRootNode->swapID(node);
                newRootNode->insert(key, emptyValue);
                newRootNode->setChildID(0, node->getID());
                newRootNode->setChildID(1, splittedNode->getID());
                node->setInternalNode();
                splittedNode->setInternalNode();
                break;
            }

            if(!nodeStack.empty()) {
                node = nodeStack.top();
                nodeStack.pop();
            }
        }
    }
    return;
}

void BTree::handleRootSplit(std::shared_ptr<BTNode> root, std::shared_ptr<Key> key, std::shared_ptr<Value> value) {
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
    std::shared_ptr<Key> medianKey = node->getKey(node->getItemCount() - 1);
    rootNode->insert(medianKey, emptyValue);
    rootNode->setChildID(0, node->getID());
    rootNode->setChildID(1, splittedNode->getID());
    rootNode->setRootNode();
}