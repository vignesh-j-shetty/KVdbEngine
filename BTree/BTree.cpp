#ifndef BTREE
#define BTREE
#include "TypeDefs.h"
class BTree {
    public:
    void insertKeyValue(char *key, uint16 keySize, char *value, uint16 valueSize);
};

#endif