#ifndef COMPARE
#define COMPARE

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Compare : public HasAdressMode, public HasFlags
{
protected:
    IAdressMode *mSrc;

public:
    Compare(Implied *src, IAdressMode *adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mSrc = src;
    }
};

#endif