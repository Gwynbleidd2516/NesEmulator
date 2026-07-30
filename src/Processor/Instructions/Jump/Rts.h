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
        addr.l = **mSP;
        (*mSP)++;
        addr.h = **mSP;
        (*mSP)++;
        *mPC = (uint8_t *)mBegin + addr.raw + 1;
    }
};

#endif