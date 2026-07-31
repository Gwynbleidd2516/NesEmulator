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
    uint8_t *mStack;
    MemoryMap *mBegin;

public:
    PullAcc(CPU *cpu, Accumulator *accumulator, uint8_t *stack, Flags *fl) : HasFlags(fl)
    {
        mAccumulator = accumulator;
        mStack = stack;
        mBegin = &cpu->memoryMap;
    }

    void code(uint8_t **) override
    {
    }

    void execute() override
    {
        mFlags->Zero = (mBegin->mMirror->stack[*mStack] == 0);
        mFlags->Negative = (mBegin->mMirror->stack[*mStack] >> 7);
        mAccumulator->setValue(mBegin->mMirror->stack[*mStack]);
        (*mStack)++;
    }
};

#endif