#include "BTree.h"
#include <string>
#include<memory>

int main() {
    //Usage Example
    BTree btree;
    std::shared_ptr<Key> key(new UIntKey(3));
    std::shared_ptr<Value> svalue(new StringValue("value"));
    btree.insert(key, svalue);
    std::shared_ptr<Value> value = btree.getValue(key);
    btree.remove(key);
    return 0;
}