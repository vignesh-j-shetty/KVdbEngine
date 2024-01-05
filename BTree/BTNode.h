#ifndef BTREE
#define BTREE
#include "TypeDefs.h"
#include "Page.h"
#include "Key.h"
#include "Value.h"
#include "KeyValueFactory.h"
#include <memory>
#define RECORD_MAX_SIZE 512
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
    std::shared_ptr<Key> getKey(uint8 index);
    std::shared_ptr<Value> getValue(uint8 index);
    uint8 getItemCount();
    private:
    std::shared_ptr<Page> page;
    char *temporaryRecordBuffer = nullptr;
    KeyValueFactory kvFactory;

    uint16 serializeToTemporaryBuffer(std::shared_ptr<Key> key, std::shared_ptr<Value> value);
};

#endif