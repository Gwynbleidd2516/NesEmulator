#ifndef INC
#define INC

#include "HasAdressMode.h"
#include "Implied.h"
#include "HasFlags.h"
#include "Absolute.h"

class Inc : public HasAdressMode, public HasFlags
{

public:
    Inc(IAdressMode *adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        // mReg = reg;
    }

    // Inc(shared_ptr<IAdressMode> adressMode) : HasAdressMode(adressMode)
    // {
    //     mReg = reg;
    // }

    void execute() override
    {
        uint8_t buf = mAdressMode->getValue() + 1;
        mAdressMode->setValue(buf + 1);
        mFlags->Zero = (buf == 0);
        mFlags->Negative = (buf >> 7);
        // (*mAdressMode->getResult())++;
    }
};

#endif