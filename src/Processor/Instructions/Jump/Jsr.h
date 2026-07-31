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
        uint16_t buf = (uint16_t)(*mPC - (uint8_t *)mBegin) - 1;
        mBegin->mMirror->stack[--(*mSP)] = buf & 0xFF;
        mBegin->mMirror->stack[--(*mSP)] = (buf >> 8) & 0xFF;
        *mPC = (uint8_t *)mBegin + mJump - 1;
    }
};

#endif