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
    StackPointer *mStack;

public:
    TransferFromStack(Index *index, StackPointer *stack, Flags *fl) : HasFlags(fl)
    {
        mIndex = index;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        mFlags->Zero = (mStack->getValue() == 0);
        mFlags->Negative = (mStack->getValue() >> 7);
        mIndex->setValue(mStack->getValue());
    }
};

#endif