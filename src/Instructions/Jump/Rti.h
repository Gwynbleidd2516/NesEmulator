#ifndef RTI
#define RTI

#include "Jump.h"
#include "HasFlags.h"

class Rti : public Jump, public HasFlags
{
public:
    Rti(vector<uint8_t>::iterator begin, StackPointer *SP, Flags *fl) : Jump(begin, nullptr, SP), HasFlags(fl)
    {
    }

    void execute() override
    {
        Flags a;
        a = mSP->pull();
        a.Break = false;
        a.Interrupt = false;
        *mFlags = a;
        uint16_t buf = 0;
        buf += mSP->pull();
        buf += mSP->pull() << 8;
        *mPC = mBegin + buf;
    }
};

#endif