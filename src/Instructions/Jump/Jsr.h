#ifndef JSR
#define JSR

#include "Jump.h"

class Jsr : public Jump
{
public:
    using Jump::Jump;

    void execute() override
    {
        uint16_t buf = *mPC - mBegin;
        mSP->push(buf >> 8);
        mSP->push(buf & 0xFF);
        *mPC = mBegin + mAdressMode->getValue();
    }
};

#endif