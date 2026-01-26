#ifndef PULL_ACCUMULATOR
#define PULL_ACCUMULATOR

#include "IInstruction.h"
#include "Accumulator.h"
#include "StackPointer.h"
#include "HasFlags.h"

class PullAcc : public IInstruction, public HasFlags
{
private:
    Accumulator *mAccumulator;
    uint8_t **mStack;

public:
    PullAcc(Accumulator *accumulator, uint8_t **stack, Flags *fl) : HasFlags(fl)
    {
        mAccumulator = accumulator;
        mStack = stack;
    }

    void code(uint8_t **) override
    {
    }

    void execute() override
    {
        (*mStack)--;
        mFlags->Zero = (**mStack == 0);
        mFlags->Negative = (**mStack >> 7);
        mAccumulator->setValue(**mStack);
        (**mStack) = 0;
        // mAccumulator->operator=(mStack->pull());
    }
};

#endif