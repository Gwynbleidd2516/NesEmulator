#ifndef BRK
#define BRK

#include "Jump.h"
#include "HasFlags.h"

class Brk : public Jump, public HasFlags
{
public:
    Brk(vector<uint8_t>::iterator begin, IAdressMode *adressMode, StackPointer *SP, Flags *fl) : Jump(begin, adressMode, SP), HasFlags(fl)
    {
    }

    void execute() override
    {
        uint16_t buf = *mPC - mBegin;
        mSP->push(buf >> 8);
        mSP->push(buf & 0xFF);
        mFlags->Interrupt = true;
        Flags a = *mFlags;
        a.Break = true;
        mSP->push(a);
        *mPC = mBegin + 0xFFFE;
    }
};

#endif