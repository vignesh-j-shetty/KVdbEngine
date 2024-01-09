#include "Page.h"
// #include "BTNode.h"
// #include "StringKey.h"
// #include "StringValue.h"
#include "DiskManager.h"
#include <string>
#include<memory>
#include<iostream>

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
    DiskManager diskmanager = DiskManager("hello.db");
    diskmanager.deletePage(41);
    return 0;
}

