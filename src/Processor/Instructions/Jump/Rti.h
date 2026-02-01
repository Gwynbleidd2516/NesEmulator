#ifndef RTI
#define RTI

#include "Jump.h"
#include "HasFlags.h"
#include "Empty.h"

class Rti : public Jump, public HasFlags
{
public:
    Rti(CPU *begin, uint8_t **SP, Flags *fl) : Jump(begin, new Empty, SP), HasFlags(fl)
    {
    }

    void execute() override
    {
        Flags a;
        a = **mSP;
        (*mSP)--;
        a.Break = false;
        a.Interrupt = false;
        *mFlags = a;
        uint16_t buf = 0;
        buf += **mSP;
        (*mSP)--;
        buf += **mSP;
        (*mSP)--;
        *mPC = (uint8_t *)mBegin + buf - 1;
    }
};

#endif