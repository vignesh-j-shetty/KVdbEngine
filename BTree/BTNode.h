#ifndef BTREE
#define BTREE
#include "TypeDefs.h"
#include "Page.h"
#include "Key.h"
#include "Value.h"
#include "KeyValueFactory.h"
#include <memory>
#define RECORD_MAX_SIZE 512
#define INTERNAL_NODE_MAX_ELEMENTS 80
class BTNode {
    public:
    BTNode(std::shared_ptr<Page> page);
    BTNode(BTNode &node) {
        temporaryRecordBuffer = new char[RECORD_MAX_SIZE];
        page = node.page;
    }
    BTNode() {

    }
    ~BTNode() {
        if(!temporaryRecordBuffer) {
            delete temporaryRecordBuffer;
        }
    }
    void insert(std::shared_ptr<Key> key, std::shared_ptr<Value> value);
    void remove(uint16 index);
    BTNode split(std::shared_ptr<Page> page);
    std::shared_ptr<Key> getKey(uint16 index);
    std::shared_ptr<Value> getValue(uint16 index);
    uint16 getItemCount();
    private:
    std::shared_ptr<Page> page;
    char *temporaryRecordBuffer = nullptr;
    KeyValueFactory kvFactory;
    uint16 serializeToTemporaryBuffer(std::shared_ptr<Key> key, std::shared_ptr<Value> value);
};

#endif