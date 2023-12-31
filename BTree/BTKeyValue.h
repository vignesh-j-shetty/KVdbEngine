#ifndef BTKEYVALUE
#define BTKEYVALUE
#include "TypeDefs.h"
#include<cstring>
enum BTKeyType {
    INT8 = 0, INT16 = 1, STR = 2
};

struct BTKeyValuePair {
    uint16 keySize;
    uint8 keyType;
    char *key;
    uint16 valueSize;
    char *value;
    // Writes Serilized data to memoryBlock assuming enough space is there
    void serialize(char *memoryBlock) {
        const char* t = memoryBlock;
        memcpy(memoryBlock, &keySize, sizeof(keySize));
        memoryBlock += sizeof(keySize);
        memcpy(memoryBlock, &valueSize, sizeof(valueSize));
        memoryBlock += sizeof(valueSize);
        memcpy(memoryBlock, &keyType, sizeof(keyType));
        memoryBlock += sizeof(keyType);
        memcpy(memoryBlock, key, keySize);
        memoryBlock += keySize;
        memcpy(memoryBlock, value, valueSize);
    }
    // Reads Serialized data from memoryBlock and returns a new BTKeyValuePair
    static BTKeyValuePair deSerialize(char* memoryBlock) {
        const char* t = memoryBlock;
        BTKeyValuePair result;

        memcpy(&result.keySize, memoryBlock, sizeof(result.keySize));
        memoryBlock += sizeof(result.keySize);
        memcpy(&result.valueSize, memoryBlock, sizeof(result.valueSize));
        memoryBlock += sizeof(result.valueSize);
        memcpy(&result.keyType, memoryBlock, sizeof(result.keyType));
        memoryBlock += sizeof(result.keyType);

        // Allocate memory for key and value based on the sizes
        result.key = new char[result.keySize];
        result.value = new char[result.valueSize];

        // Copy key and value data
        memcpy(result.key, memoryBlock, result.keySize);
        memoryBlock += result.keySize;
        memcpy(result.value, memoryBlock, result.valueSize);

        return result;
    }
};
#endif
