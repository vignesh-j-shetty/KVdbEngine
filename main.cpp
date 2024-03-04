#include "BTree.h"
#include <string>
#include<memory>
#include<iostream>
#include <chrono>

// void print(BTNode &node) {
//     for(uint8 i = 0; i < node.getItemCount(); i++) {
//         std::string key = std::any_cast<std::string>(node.getKey(i)->getData());
//         std::string value = std::any_cast<std::string>(node.getValue(i)->getValue());
//         std::cout<<"Key :"<<key<<" -- ";
//         std::cout<<"Value :"<<value<<std::endl;
//     }
// }

//auto start = std::chrono::high_resolution_clock::now();
// auto end = std::chrono::high_resolution_clock::now();
// std::chrono::duration<double, std::milli> ms_double = end - start;
// std::cout << "Execution time: " << ms_double.count() << " milliseconds\n";


void setup(BTree &btree) {
    std::shared_ptr<Key> key(new StringKey("Hello40"));
    std::shared_ptr<Value> value(new StringValue("fsdfs"));
    // Insert 1
    btree.insert(key, value);

    // Insert 1
    key->setData(std::any_cast<std::string>(std::string("Hello50")));
    btree.insert(key, value);

    // Insert 2
    key->setData(std::any_cast<std::string>(std::string("Hello30")));
    btree.insert(key, value);

    // Insert 3
    key->setData(std::any_cast<std::string>(std::string("Hello70")));
    btree.insert(key, value);
}

int main() {
    BTree btree;
    if(false) {
        setup(btree);
    } else {
        std::shared_ptr<Key> key(new StringKey("Hello60"));
        std::shared_ptr<Value> value(new StringValue("fsdfs"));
        btree.insert(key, value);
    }
    btree.debugPrint();
    return 0;
}

