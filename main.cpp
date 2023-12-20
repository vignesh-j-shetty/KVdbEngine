#include "DiskManager.h"

int main() {
    //Testing
    DiskManager *dm = new DiskManager("test.db");
    dm->createFreePage();
    delete dm;
    return 0;
}