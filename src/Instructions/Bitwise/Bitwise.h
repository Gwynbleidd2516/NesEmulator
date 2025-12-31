#ifndef BITWISE
#define BITWISE

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Bitwise : public HasAdressMode, public HasFlags
{
protected:
    Accumulator *mReg;

public:
    Bitwise(Accumulator *reg, IAdressMode *adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mReg = reg;
    }
};

#endif