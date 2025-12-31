#ifndef PULL_PROCESSOR_STATUS
#define PULL_PROCESSOR_STATUS

#include "IInstruction.h"
#include "Structs.h"
#include "StackPointer.h"

class PullProcStatus : public IInstruction
{
private:
    Flags *mFlags;
    StackPointer *mStack;

public:
    PullProcStatus(Flags *flags, StackPointer *stack)
    {
        mFlags = flags;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        *mFlags = mStack->pull();
    }
};

#endif