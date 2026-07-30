#ifndef TRANSFER_TO_STACK
#define TRANSFER_TO_STACK

#include "IInstruction.h"
#include "Index.h"
#include "StackPointer.h"

class TransferToStack : public IInstruction
{
private:
    Index *mIndex;
    uint8_t **mStack;
    CPU *mBegin;

public:
    TransferToStack(CPU *cpu, Index *index, uint8_t **stack)
    {
        mIndex = index;
        mStack = stack;
        mBegin = cpu;
    }

    void code(uint8_t **) override
    {
    }

    void execute() override
    {
        *mStack = (uint8_t *)mBegin->memoryMap.mMirror->stack + mIndex->getValue();
    }
};

#endif