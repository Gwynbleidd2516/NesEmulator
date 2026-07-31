#ifndef PULL_PROCESSOR_STATUS
#define PULL_PROCESSOR_STATUS

#include "IInstruction.h"

class PullProcStatus : public IInstruction
{
private:
    Flags *mFlags;
    uint8_t *mStack;
    MemoryMap *mBegin;

public:
    PullProcStatus(CPU *cpu, Flags *flags, uint8_t *stack)
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
        mFlags->raw = mBegin->mMirror->stack[(*mStack)++];
    }
};

#endif