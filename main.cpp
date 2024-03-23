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

// auto start = std::chrono::high_resolution_clock::now();
// auto end = std::chrono::high_resolution_clock::now();
// std::chrono::duration<double, std::milli> ms_double = end - start;
// std::cout << "Execution time: " << ms_double.count() << " milliseconds\n";


void setup(BTree &btree) {
    std::shared_ptr<Key> key(new UIntKey(0));
    std::shared_ptr<Value> value(new StringValue("fsdfs"));

    for (uint64 i = 0; i < 35; i++) {
        key->setData(std::any_cast<uint64>(i));
        btree.insert(key, value);
    }
}

int main() {
    BTree btree;
    setup(btree);
    btree.debugPrint();
    return 0;
}

