#ifndef TRANSFER_FROM_STACK
#define TRANSFER_FROM_STACK

#include "IInstruction.h"
#include "Index.h"
#include "StackPointer.h"
#include "HasFlags.h"

class TransferFromStack : public IInstruction, public HasFlags
{
private:
    Index *mIndex;
    uint8_t **mStack;

public:
    TransferFromStack(Index *index, uint8_t **stack, Flags *fl) : HasFlags(fl)
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
        mFlags->Zero = (**mStack == 0);
        mFlags->Negative = (**mStack >> 7);
        mIndex->setValue(**mStack);
        (*mStack)++;
    }
};

#endif