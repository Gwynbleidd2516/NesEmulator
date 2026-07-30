#ifndef JUMP
#define JUMP

#include "HasAdressMode.h"
#include "StackPointer.h"

class Jump : public HasAdressMode
{
protected:
    uint8_t **mPC;
    MemoryMap *mBegin;
    uint8_t **mSP;
    uint16_t mJump;

public:
    Jump(CPU *begin, shared_ptr<IAdressMode> adressMode, uint8_t **SP) : HasAdressMode(adressMode)
    {
        mBegin = &begin->memoryMap;
        mSP = SP;
    }

    void code(uint8_t **it) override
    {
        mAdressMode->setJumpPointer(&mJump);
        mAdressMode->code(it);
        mPC = it;
    }
};

#endif