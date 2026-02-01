#ifndef NOP
#define NOP

#include "IInstruction.h"

class Nop : public IInstruction
{
public:
    void execute() override
    {
    }

    void code(uint8_t **) override
    {
    }
};

#endif