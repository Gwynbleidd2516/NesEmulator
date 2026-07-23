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
        mFlags->Carry = (am >= src);
        mFlags->Zero = (am == src);
        mFlags->Negative = (am < src);
    }
};

#endif