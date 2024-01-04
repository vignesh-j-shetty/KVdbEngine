#include "Page.h"
#include "BTNode.h"
#include "StringKey.h"
#include "StringValue.h"
#include <string>
#include<memory>
#include<iostream>

int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
        buffer[i] = 0;
    }
    std::string valueString = "sncknck";
    std::shared_ptr<Page> page(new Page(buffer, 2024));
    BTNode node(page);
    std::shared_ptr<Key> key(new StringKey("AAaa"));
    std::shared_ptr<Value> value(new StringValue("ABCDEF"));
    node.insert(key, value);
    key->setData(std::string("zzzz"));
    node.insert(key, value);
    key->setData(std::string("Aaaa"));
    node.insert(key, value);
    node.printKeys();
    return 0;
}

