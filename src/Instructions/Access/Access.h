#ifndef ACCESS
#define ACCESS

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Access : public HasAdressMode, public HasFlags
{
protected:
    Implied *mReg;

public:
    Access(Implied *reg, IAdressMode *adressMode, Flags *flags) : HasAdressMode(adressMode), HasFlags(flags)
    {
        mReg = reg;
    }

    ~Access()
    {
        delete mReg;
    }
};

#endif