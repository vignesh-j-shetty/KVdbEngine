#include "Page.h"
#include "BTNode.h"
#include "StringKey.h"
#include "StringValue.h"
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


int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    memset(buffer, 0, DISKMANAGER_PAGESIZE);
    std::shared_ptr<Page> page(new Page(buffer, 0));
    std::string s = "key";
    page->insertRecord(s.data(), 3);
    return 0;
}

