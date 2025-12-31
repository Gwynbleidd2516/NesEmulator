#ifndef NOP
#define NOP

#include "IInstruction.h"

class Nop : public IInstruction
{
public:
    void execute() override
    {
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }
};

#endif