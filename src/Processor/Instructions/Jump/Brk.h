#ifndef BRK
#define BRK

#include "Jump.h"
#include "HasFlags.h"

class Brk : public Jump, public HasFlags
{
public:
    Brk(CPU *begin, shared_ptr<IAdressMode> adressMode, uint8_t **SP, Flags *fl) : Jump(begin, adressMode, SP), HasFlags(fl)
    {
    }

    void execute() override
    {
        uint16_t buf = (uint16_t)(*mPC - (uint8_t *)mBegin) + 1;
        (*mSP)--;
        **mSP = buf & 0xFF;
        (*mSP)--;
        **mSP = (buf >> 8) & 0xFF;
        mFlags->Interrupt = true;
        Flags a = *mFlags;
        a.Break = true;
        (*mSP)--;
        **mSP = a.raw;
        *mPC = (uint8_t *)mBegin + mBegin->irqVector.raw;
    }
};

#endif