#ifndef PUSH_ACCUMULATOR
#define PUSH_ACCUMULATOR

#include "IInstruction.h"
#include "Accumulator.h"
#include "StackPointer.h"

class PushAcc : public IInstruction
{
private:
    Accumulator *mAccumulator;
    uint8_t **mStack;

public:
    PushAcc(Accumulator *accumulator, uint8_t **stack)
    {
        mAccumulator = accumulator;
        mStack = stack;
    }

    void code(uint8_t **) override
    {
    }

    void execute() override
    {
        **mStack = mAccumulator->getValue();
        (*mStack)++;
    }
};

#endif