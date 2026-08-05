#ifndef PATTERN
#define PATTERN

#include <stdint.h>

union Pattern
{
    struct
    {
        unsigned int layer11;
        unsigned int layer12;
        unsigned int layer21;
        unsigned int layer22;
    };
    struct 
    {
        uint8_t layer1[8];
        uint8_t layer2[8];
    };    
};

#endif