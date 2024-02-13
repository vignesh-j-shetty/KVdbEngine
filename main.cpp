#include "Page.h"
#include "BTNode.h"
#include "StringKey.h"
#include "StringValue.h"
#include "DiskManager.h"
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
int main() {
    DiskManager diskmanger("test.db");
    std::shared_ptr<Page> page = diskmanger.createPage();
    BTNode node(page);
    std::shared_ptr<Key> key(new StringKey("Hevhvhfhfghfhfe"));
    std::shared_ptr<Value> value(new StringValue("he"));
    try {
        node.insert(key, value);
    } catch (NoSpaceException e) {
        std::cout<<"Hellow"<<std::endl;
    }
    return 0;
}

