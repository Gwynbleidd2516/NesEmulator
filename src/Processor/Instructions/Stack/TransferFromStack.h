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
    CPU *mBegin;

public:
    TransferFromStack(CPU *cpu, Index *index, uint8_t **stack, Flags *fl) : HasFlags(fl)
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
        uint8_t buf = (uint8_t *)mBegin->memoryMap.mMirror->stack - *mStack;
        mFlags->Zero = (buf == 0);
        mFlags->Negative = (buf >> 7);
        mIndex->setValue(buf);
    }
};

#endif