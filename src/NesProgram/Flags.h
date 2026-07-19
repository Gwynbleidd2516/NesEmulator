#ifndef FLAGS
#define FLAGS

#include <cstring>
#include <stdint.h>

struct Flags
{
    bool Negative : 1;
    bool Overflow : 1;
    bool Break : 1;
    bool Decimal : 1;
    bool Interrupt : 1;
    bool Zero : 1;
    bool Carry : 1;
};

#endif