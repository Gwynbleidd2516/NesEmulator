#ifndef RTS
#define RTS

#include "Jump.h"

class Rts : public Jump
{
public:
    using Jump::Jump;

    void execute() override
    {
        uint16_t buf = 0;
        (*mSP)--;
        buf += **mSP;
        (*mSP)--;
        buf += **mSP << 8;
        *mPC = (uint8_t *)mBegin + buf;
    }
};

#endif