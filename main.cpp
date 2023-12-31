#include "DiskManager.h"
#include "Page.h"
#include "BTNode.h"
#include <string>
#include<memory>
BTKeyValuePair getKeyValuePair(std::string key, std::string value) {
    BTKeyValuePair keyValuePair;
    keyValuePair.keyType = STR;
    keyValuePair.keySize = key.size() + 1;
    keyValuePair.valueSize = value.size() + 1;
    keyValuePair.key = key.data();
    keyValuePair.value = value.data();
    return keyValuePair;
}

int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
        buffer[i] = 0;
    }
    //buffer[DISKMANAGER_PAGESIZE] = 'Z';
    //buffer[DISKMANAGER_PAGESIZE + 1] = 'Z';
    Page page = Page(buffer, 2024);
    // BTNode node(page);
    // BTKeyValuePair pair1 = getKeyValuePair("def", "wsafsf");
    // BTKeyValuePair pair2 = getKeyValuePair("abc", "wsafsf");
    // node.insertKeyValue(pair1);
    // node.insertKeyValue(pair2);
    return 0;
}

