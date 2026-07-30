#ifndef DEC
#define DEC

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Dec : public HasAdressMode, public HasFlags
{
public:
    Dec(shared_ptr<IAdressMode> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
    }

    void execute() override
    {
        mAdressMode->setValue(mAdressMode->getValue() - 1);
        mFlags->Zero = (mAdressMode->getValue() == 0);
        mFlags->Negative = (mAdressMode->getValue() >> 7);
    }
};

#endif