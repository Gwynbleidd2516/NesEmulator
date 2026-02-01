#ifndef LOAD
#define LOAD

#include "Access.h"

class Load : public Access
{
public:
    using Access::Access;

    void execute() override
    {
        mFlags->Zero = (mAdressMode->getValue() == 0);
        mFlags->Negative = (mAdressMode->getValue() >> 7);
        mReg->setValue(mAdressMode->getValue());
    }
};

#endif