#include "DiskManager.h"

int main() {
    //Testing
    DiskManager *dm = new DiskManager("test.db");
    char *buffer = new char[2024];
    Page page = dm->readPage(16);
    delete dm;
    return 0;
}