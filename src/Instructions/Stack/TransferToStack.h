#ifndef TRANSFER_TO_STACK
#define TRANSFER_TO_STACK

#include "IInstruction.h"
#include "Index.h"
#include "StackPointer.h"

class TransferToStack : public IInstruction
{
private:
    Index *mIndex;
    StackPointer *mStack;

public:
    TransferToStack(Index *index, StackPointer *stack)
    {
        mIndex = index;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        mStack->setValue(mIndex->getValue());
    }
};

#endif