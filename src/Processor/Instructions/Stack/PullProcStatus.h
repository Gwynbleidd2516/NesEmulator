#ifndef PULL_PROCESSOR_STATUS
#define PULL_PROCESSOR_STATUS

#include "IInstruction.h"
#include "Structs.h"
#include "StackPointer.h"

class PullProcStatus : public IInstruction
{
private:
    Flags *mFlags;
    uint8_t **mStack;

public:
    PullProcStatus(Flags *flags, uint8_t **stack)
    {
        mFlags = flags;
        mStack = stack;
    }

    void code(uint8_t **) override
    {
    }

    void execute() override
    {
        (*mStack)--;
        *mFlags = **mStack;
    }
};

#endif