#ifndef BRK
#define BRK

#include "Jump.h"
#include "HasFlags.h"

class Brk : public Jump, public HasFlags
{
public:
    Brk(CPU *begin, IAdressMode *adressMode, uint8_t **SP, Flags *fl) : Jump(begin, adressMode, SP), HasFlags(fl)
    {
    }

    void execute() override
    {
        uint16_t buf = *mPC - (uint8_t *)mBegin;
        **mSP = buf >> 8;
        (*mSP)++;
        **mSP = buf & 0xFF;
        (*mSP)++;
        mFlags->Interrupt = true;
        Flags a = *mFlags;
        a.Break = true;
        (**mSP) = a;
        (*mSP)++;
        *mPC = (uint8_t *)mBegin + 0xFFFE;
    }
};

#endif