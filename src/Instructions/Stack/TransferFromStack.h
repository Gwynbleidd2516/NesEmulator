#ifndef TRANSFER_FROM_STACK
#define TRANSFER_FROM_STACK

#include "IInstruction.h"
#include "Index.h"
#include "StackPointer.h"

class TransferFromStack : public IInstruction
{
private:
    Index *mIndex;
    StackPointer *mStack;

public:
    TransferFromStack(Index *index, StackPointer *stack)
    {
        mIndex = index;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        mIndex->setValue(mStack->getValue());
    }
};

#endif