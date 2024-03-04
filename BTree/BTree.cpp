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
        handleSplit(currentNode, nodeStack);
        std::shared_ptr<BTNode> parentNode;
        if(nodeStack.empty()) {
            parentNode = bufferPoolManager->getRootPage();
        } else {
            parentNode = nodeStack.top(); 
        }
        uint16 index = parentNode->searchCmp(key);
        std::shared_ptr<BTNode> nodeToInsert = bufferPoolManager->getNode(parentNode->getChildID(index));
        nodeToInsert->insert(key, value);
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

void BTree::handleSplit(std::shared_ptr<BTNode> node, std::stack<std::shared_ptr<BTNode>> &nodeStack) {
    std::shared_ptr<BTNode> split1 = node;
    std::shared_ptr<BTNode> split2 = bufferPoolManager->newNode();
    split1->split(split2);
    std::shared_ptr<Key> key = split1->getKey(split1->getItemCount() - 1);

    if(node->isInternalNode()) {
        node->remove(split1->getItemCount() - 1);
    }
    // Handles the case when only one node is there
    if(nodeStack.size() == 0) {
        std::shared_ptr<BTNode> newRootNode = bufferPoolManager->newNode();
        split1->swapID(newRootNode);
        newRootNode->insert(key, emptyValue);
        newRootNode->setChildID(0, split1->getID());
        newRootNode->setChildID(1, split2->getID());

        split1->setLeafNode();
        split2->setLeafNode();
        return;
    }

}