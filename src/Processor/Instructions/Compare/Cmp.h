#ifndef CMP
#define CMP

#include "Compare.h"
#include "Implied.h"

class Cmp : public Compare
{
public:
    using Compare::Compare;

    // Cmp(shared_ptr<Implied> reg, shared_ptr<IAdressMode> adressMode) : HasAdressMode(adressMode)
    // {
    //     mReg = reg;
    // }

    void execute() override
    {
        if (mAdressMode->getValue() >= mSrc->getValue())
        {
            mFlags->Carry = true;
        }
        else if (mAdressMode->getValue() == mSrc->getValue())
        {
            mFlags->Zero = true;
        }
        else
        {
            mFlags->Negative = true;
        }
    }
};

#endif