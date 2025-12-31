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
    StackPointer *mStack;

public:
    PullAcc(Accumulator *accumulator, StackPointer *stack, Flags *fl) : HasFlags(fl)
    {
        mAccumulator = accumulator;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        mFlags->Zero = (mStack->getValue() == 0);
        mFlags->Negative = (mStack->getValue() >> 7);
        mAccumulator->setValue(mStack->pull());
        // mAccumulator->operator=(mStack->pull());
    }
};

#endif