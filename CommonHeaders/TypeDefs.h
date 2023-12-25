#ifndef TYPEDEF
#define TYPEDEF
#include <stdint.h>
#define MAX_INT_16 65535
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

enum KeyType {
    STRING = 0, INTEGER = 1
};

#endif