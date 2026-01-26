#ifndef JSR
#define JSR

#include "Jump.h"

class Jsr : public Jump
{
public:
    using Jump::Jump;

    void execute() override
    {
        uint16_t buf = *mPC - (uint8_t *)mBegin;
        **mSP = buf >> 8;
        (*mSP)++;
        **mSP = (buf & 0xFF);
        (*mSP)++;
        *mPC = (uint8_t *)mBegin + mAdressMode->getValue();
        // *mPC = mAdressMode->getValue();
    }
};

#endif