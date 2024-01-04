#ifndef BTREE
#define BTREE
#include "TypeDefs.h"
#include "Page.h"
#include "Key.h"
#include "Value.h"
#include <memory>
#define RECORD_MAX_SIZE 512

class BTNode {
    public:
    BTNode(std::shared_ptr<Page> page);
    BTNode(BTNode &node) {
        temporaryRecordBuffer = new char[RECORD_MAX_SIZE];
        page = node.page;
    }
    ~BTNode() {
        delete temporaryRecordBuffer;
    }
    void insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value);
    uint8 getItemCount();
    void printKeys();
    uint16 serializeToTemporaryBuffer(std::shared_ptr<Key> key, std::shared_ptr<Value> value);
    private:
    std::shared_ptr<Page> page;
    char *temporaryRecordBuffer = nullptr;
};

#endif