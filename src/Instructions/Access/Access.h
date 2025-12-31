#ifndef ACCESS
#define ACCESS

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Access : public HasAdressMode, public HasFlags
{
protected:
    shared_ptr<Implied> mReg;

public:
    Access(shared_ptr<Implied> reg, IAdressMode* adressMode, Flags *flags) : HasAdressMode(adressMode), HasFlags(flags)
    {
        mReg = reg;
    }
};

#endif