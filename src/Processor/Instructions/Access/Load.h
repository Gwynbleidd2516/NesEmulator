#ifndef LOAD
#define LOAD

#include "Access.h"

class Load : public Access
{
public:
    using Access::Access;

    void execute() override
    {
        uint8_t buf = mAdressMode->getValue();
        mFlags->Zero = !buf;
        mFlags->Negative = (buf >> 7);
        mReg->setValue(buf);
    }
};

#endif