#ifndef RTI
#define RTI

#include "Jump.h"
#include "HasFlags.h"
#include "Empty.h"
#include "Struct16_t.h"

class Rti : public Jump, public HasFlags
{
public:
    Rti(CPU *begin, uint8_t *SP, Flags *fl) : Jump(begin, make_shared<Empty>(), SP), HasFlags(fl)
    {
    }

    void execute() override
    {
        mFlags->raw = mBegin->mMirror->stack[(*mSP)++];
        mFlags->Break = false;
        mFlags->Interrupt = false;
        Struct16_t buf;
        buf.l = mBegin->mMirror->stack[(*mSP)++];
        buf.h = mBegin->mMirror->stack[(*mSP)++];
        *mPC = (uint8_t *)mBegin + buf.raw - 1;
        mBegin->mPPURegs->ppuctrl.nmi_enable = true;
    }
};

#endif