#ifndef ATTRIBUTE_TABLE
#define ATTRIBUTE_TABLE

#include <stdint.h>

struct AttributeTable
{
    uint8_t topLeft : 2;
    uint8_t topRight : 2;
    uint8_t bottomLeft : 2;
    uint8_t bottomRight : 2;
};


#endif