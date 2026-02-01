#ifndef EOR
#define EOR

#include "Bitwise.h"

class Eor : public Bitwise
{
public:
    using Bitwise::Bitwise;

    void execute() override
    {
        mReg->setValue(mReg->getValue() ^ mAdressMode->getValue());
        mFlags->Zero = (mReg->getValue() == 0);
        mFlags->Negative = (mReg->getValue() >> 7);
        // *(mReg->getResult()) ^= *(mAdressMode->getResult());
    }
};

#endif