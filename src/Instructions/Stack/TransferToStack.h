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

public:
    TransferToStack(Index *index, uint8_t **stack)
    {
        mIndex = index;
        mStack = stack;
    }

    void code(uint8_t **) override
    {
    }

    void execute() override
    {
        (*mStack)--;
        **mStack = mIndex->getValue();
        (*mStack)++;
    }
};

#endif