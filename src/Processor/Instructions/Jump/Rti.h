#ifndef RTI
#define RTI

#include "Jump.h"
#include "HasFlags.h"
#include "Empty.h"
#include"Struct16_t.h"

class Rti : public Jump, public HasFlags
{
public:
    Rti(CPU *begin, uint8_t **SP, Flags *fl) : Jump(begin, new Empty, SP), HasFlags(fl)
    {
    }

    void execute() override
    {
        Flags a;
        a.raw = **mSP;
        (*mSP)--;
        a.Break = false;
        a.Interrupt = false;
        *mFlags = a;
        Struct16_t buf;
        buf.l = **mSP;
        (*mSP)--;
        buf.h = **mSP;
        (*mSP)--;
        *mPC = (uint8_t *)mBegin + buf.raw - 1;
        mBegin->mPPURegs->ppuctrl.nmi_enable = true;
    }
};

#endif