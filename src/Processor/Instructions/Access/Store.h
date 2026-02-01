#ifndef STORE
#define STORE

#include "Access.h"

class Store : public Access
{
public:
    using Access::Access;

    void execute() override
    {
        mFlags->Zero = (mReg->getValue() == 0);
        mFlags->Negative = (mReg->getValue() >> 7);
        mAdressMode->setValue(mReg->getValue());
    }
};

#endif