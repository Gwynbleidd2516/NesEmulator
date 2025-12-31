#ifndef BIT
#define BIT

#include "Bitwise.h"

class Bit : public Bitwise
{
public:
    using Bitwise::Bitwise;

    void execute() override
    {
        uint8_t res = mReg->getValue() & mAdressMode->getValue();
        mFlags->Zero = (res == 0);
        mFlags->Overflow = ((res >> 6) % 2);
        mFlags->Negative = (res >> 7);
        // *(mReg->getResult()) & *(mAdressMode->getResult());
    }
};

#endif