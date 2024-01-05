#include "Page.h"
#include "BTNode.h"
#include "StringKey.h"
#include "StringValue.h"
#include <string>
#include<memory>
#include<iostream>

void print(BTNode &node) {
    for(uint8 i = 0; i < node.getItemCount(); i++) {
        std::string key = std::any_cast<std::string>(node.getKey(i)->getData());
        std::string value = std::any_cast<std::string>(node.getValue(i)->getValue());
        std::cout<<"Key :"<<key<<" -- ";
        std::cout<<"Value :"<<value<<std::endl;
    }
}

int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
        buffer[i] = 0;
    }
    std::string s = "sscsdcsd";
    std::shared_ptr<Page> page(new Page(buffer, 2024));
    BTNode node(page);
    std::shared_ptr<Key> key(new StringKey("Abcd"));
    std::shared_ptr<Value> value(new StringValue("ABCDEF"));
    node.insert(key, value);
    value->setValue(s);
    key->setData(std::string("Abcz"));
    node.insert(key, value);
    key->setData(std::string("Aaaa"));
    node.insert(key, value);
    node.insert(key, value);
    print(node);
    return 0;
}

