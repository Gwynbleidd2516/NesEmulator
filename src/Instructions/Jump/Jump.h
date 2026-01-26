#ifndef JUMP
#define JUMP

#include "HasAdressMode.h"
#include "StackPointer.h"

class Jump : public HasAdressMode
{
protected:
    uint8_t **mPC;
    CPU *mBegin;
    uint8_t **mSP;

public:
    Jump(CPU *begin, IAdressMode *adressMode, uint8_t **SP) : HasAdressMode(adressMode)
    {
        mBegin = begin;
        mSP = SP;
    }

    void code(uint8_t **it) override
    {
        mAdressMode->code(it);
        mPC = it;
    }
};

#endif