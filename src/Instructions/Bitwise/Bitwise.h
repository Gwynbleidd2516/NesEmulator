#ifndef BITWISE
#define BITWISE

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Bitwise : public HasAdressMode, public HasFlags
{
protected:
    shared_ptr<Implied> mReg;

public:
    Bitwise(shared_ptr<Implied> reg, shared_ptr<IAdressMode> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mReg = reg;
    }
};

#endif