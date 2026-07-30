#ifndef COMPARE
#define COMPARE

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Compare : public HasAdressMode, public HasFlags
{
protected:
    shared_ptr<IAdressMode> mSrc;

public:
    Compare(shared_ptr<IAdressMode> src, shared_ptr<IAdressMode> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mSrc = src;
    }
};

#endif