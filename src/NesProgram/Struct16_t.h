#ifndef STRUCT16_T
#define STRUCT16_T

#include <stdint.h>

union Struct16_t
{
    struct
    {
        uint8_t h;
        uint8_t l;
    };
    uint16_t raw;
};

#endif