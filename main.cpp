#include "DiskManager.h"
#include "Page.h"
#include <string>
#include<memory>

int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
        buffer[i] = 0;
    }
    buffer[DISKMANAGER_PAGESIZE] = 'Z';
    buffer[DISKMANAGER_PAGESIZE + 1] = 'Z';
    std::string h1 = "abc";
    std::string h2 = "xyzaa";
    Page page = Page(buffer, 2024);
    page.insertRecord(h1.data(), h1.size());
    page.insertRecord(h2.data(), h2.size());
    //page.insertRecord(h3.data(), h3.size());
    return 0;
}

