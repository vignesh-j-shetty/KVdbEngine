#include "Page.h"
#include "BTNode.h"
#include "StringKey.h"
#include "StringValue.h"
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
// 16 41 66


int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
        buffer[i] = 0;
    }
    std::shared_ptr<Page> page(new Page(buffer, 0));
    char *c = "0123";
    page->insertRecord(c, 4);
    page->removeRecord(0);
    return 0;
}

