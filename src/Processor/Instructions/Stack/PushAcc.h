#ifndef PUSH_ACCUMULATOR
#define PUSH_ACCUMULATOR

#include "IInstruction.h"
#include "Accumulator.h"
#include "StackPointer.h"

class PushAcc : public IInstruction
{
private:
    Accumulator *mAccumulator;
    uint8_t *mStack;
    MemoryMap *mBegin;

public:
    PushAcc(CPU *cpu, Accumulator *accumulator, uint8_t *stack)
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
        mBegin->mMirror->stack[--(*mStack)] = mAccumulator->getValue();
    }
};

#endif