#ifndef PUSH_ACCUMULATOR
#define PUSH_ACCUMULATOR

#include "IInstruction.h"
#include "Accumulator.h"
#include "StackPointer.h"

class PushAcc : public IInstruction
{
private:
    Accumulator *mAccumulator;
    StackPointer *mStack;

public:
    PushAcc(Accumulator *accumulator, StackPointer *stack)
    {
        mAccumulator = accumulator;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        mStack->push(mAccumulator->getValue());
    }
};

#endif