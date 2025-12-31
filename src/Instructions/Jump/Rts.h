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
        buf += mSP->pull();
        buf += mSP->pull() << 8;
        *mPC = mBegin + buf;
    }
};

#endif