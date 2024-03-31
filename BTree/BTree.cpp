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
    //std::cout<<" | id : "<<node->getID()<<">";
    std::cout<<" | ";
    if(node->getNodeType() == LEAF_NODE) {
        std::cout<<"L ";
    }
    uint16 n = node->getItemCount();
    for (uint16 i = 0; i < n; i++) {
        uint64 keyValue = std::any_cast<uint64>(node->getKey(i)->getData());
        std::cout<<keyValue;
        if(i != n - 1) {
            std::cout<<" - ";
        }

    }
    std::cout<<" | ";
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