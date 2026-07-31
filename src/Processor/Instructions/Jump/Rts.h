#ifndef RTS
#define RTS

#include "Jump.h"

class Rts : public Jump
{
public:
    using Jump::Jump;

    void execute() override
    {
        Struct16_t addr;
        addr.l = mBegin->mMirror->stack[(*mSP)++];
        addr.h = mBegin->mMirror->stack[(*mSP)++];
        *mPC = (uint8_t *)mBegin + addr.raw + 1;
    }
};

#endif