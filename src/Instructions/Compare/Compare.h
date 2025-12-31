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
    Compare(shared_ptr<Implied> adressMode, shared_ptr<IAdressMode> src, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mSrc = src;
    }

    // void code(vector<uint8_t>::iterator &it) override
    // {
    //     mAdressMode->code(it);
    // }
};

#endif