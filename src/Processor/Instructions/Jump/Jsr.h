#ifndef JSR
#define JSR

#include "Jump.h"
#include <iostream>
using namespace std;

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
        *mPC = (uint8_t *)mBegin + mJump - 1;
        // *mPC = mAdressMode->getValue();
    }
};

#endif