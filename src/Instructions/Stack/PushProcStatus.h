#ifndef PUSH_PROCESSOR_STATUS
#define PUSH_PROCESSOR_STATUS

#include "IInstruction.h"
#include "Structs.h"
#include "StackPointer.h"

class PushProcStatus : public IInstruction
{
private:
    Flags *mFlags;
    StackPointer *mStack;

public:
    PushProcStatus(Flags *flags, StackPointer *stack)
    {
        mFlags = flags;
        mStack = stack;
    }

    void code(vector<uint8_t>::iterator &) override
    {
    }

    void execute() override
    {
        mStack->push(static_cast<uint8_t>(*mFlags));
    }
};

#endif