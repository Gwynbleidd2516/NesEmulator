#ifndef PUSH_PROCESSOR_STATUS
#define PUSH_PROCESSOR_STATUS

#include "IInstruction.h"

class PushProcStatus : public IInstruction
{
private:
    Flags *mFlags;
    uint8_t *mStack;
    MemoryMap *mBegin;

public:
    PushProcStatus(CPU *cpu, Flags *flags, uint8_t *stack)
    {
        mFlags = flags;
        mStack = stack;
        mBegin = &cpu->memoryMap;
    }

    void code(uint8_t **) override
    {
    }

    void execute() override
    {
        Flags buf = *mFlags;
        buf.Break = true;
        mBegin->mMirror->stack[--(*mStack)] = buf.raw;
    }
};

#endif