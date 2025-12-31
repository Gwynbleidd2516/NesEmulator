#ifndef PULL_ACCUMULATOR
#define PULL_ACCUMULATOR

#include "IInstruction.h"
#include "Accumulator.h"
#include "StackPointer.h"

class PullAcc : public IInstruction
{
private:
    Accumulator *mAccumulator;
    StackPointer *mStack;

public:
    PullAcc(Accumulator *accumulator, StackPointer *stack)
    {
        mAccumulator = accumulator;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        // mAccumulator->setValue(mStack->pull());
        mAccumulator->operator=(mStack->pull());
    }
};

#endif