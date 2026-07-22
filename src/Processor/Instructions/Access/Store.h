#ifndef STORE
#define STORE

#include "Access.h"

class Store : public Access
{
public:
    using Access::Access;

    void execute() override
    {
        uint8_t buf = mReg->getValue();
        mFlags->Zero = (buf == 0);
        mFlags->Negative = (buf >> 7);
        mAdressMode->setValue(buf);
    }
};

#endif