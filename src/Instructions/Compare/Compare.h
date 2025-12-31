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
    Compare(Implied *adressMode, IAdressMode *src, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mSrc = src;
    }

    // void code(vector<uint8_t>::iterator &it) override
    // {
    //     mAdressMode->code(it);
    // }
};

#endif