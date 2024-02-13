#ifndef _BTREE_
#define _BTREE_
#include "Key.h"
#include "Value.h"
#include "BTNodeBufferPoolManager.h"
#include<memory>

class BTree {
    public:
    void insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value);
    private:
    BTNodeBufferPoolManager bufferPoolManager;
};

#endif