#ifndef AND
#define AND

#include "Bitwise.h"

class And : public Bitwise
{
public:
    using Bitwise::Bitwise;

    void execute() override
    {
        mReg->setValue(mReg->getValue() & mAdressMode->getValue());
        mFlags->Zero = (mReg->getValue() == 0);
        mFlags->Negative = (mReg->getValue() >> 7);
        // mReg->setValue(mReg->getValue() & mAdressMode->getValue());
        // *(mReg->getResult()) &= *(mAdressMode->getResult());
    }
};

#endif