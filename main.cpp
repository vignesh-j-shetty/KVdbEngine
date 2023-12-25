#include "DiskManager.h"
#include "Page.h"
int main() {
    //Testing
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
        buffer[i] = 0;
    }
    buffer[DISKMANAGER_PAGESIZE] = 'Z';
    buffer[DISKMANAGER_PAGESIZE + 1] = 'Z';
    Page page = Page(buffer, 2024);
    page.allocateMemoryBlock(5);
    page.allocateMemoryBlock(2);
    page.dellocateMemoryBlock(0);
    //page.dellocateMemoryBlock(0);
    page.findBestFitFreeMemoryBlock(5 + 2);
    return 0;
}