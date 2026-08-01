#ifndef CMP
#define CMP

#include "Compare.h"
#include "Implied.h"

class Cmp : public Compare
{
public:
    using Compare::Compare;

    void execute() override
    {
        uint8_t am = mAdressMode->getValue();
        uint8_t src = mSrc->getValue();
        uint8_t result = am - src;
        mFlags->Carry = (am >= src);
        mFlags->Zero = (am == src);
        mFlags->Negative = (result & 0x80);
    }
};

#endif