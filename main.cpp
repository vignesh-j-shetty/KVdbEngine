#include "DiskManager.h"
#include "Page.h"
int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
        buffer[i] = 0;
    }
    Page page = Page(buffer, 2024);
    page.allocateMemoryBlock(2);
    page.allocateMemoryBlock(5);
    return 0;
}