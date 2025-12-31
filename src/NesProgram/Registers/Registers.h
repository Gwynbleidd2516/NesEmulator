#ifndef REGISTERS
#define REGISTERS

#include "Accumulator.h"
#include "Index.h"
#include "StackPointer.h"
#include "Flags.h"

struct Registers
{
    Accumulator A;
    Index X;
    Index Y;
    Flags flags;
    StackPointer sp;
};

#endif