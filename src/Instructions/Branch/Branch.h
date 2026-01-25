#ifndef BRANCH
#define BRANCH

#include "HasAdressMode.h"
#include "HasFlags.h"

class Branch : public HasAdressMode, public HasFlags
{
protected:
    vector<uint8_t>::iterator *mPC;
    // vector<uint8_t>::iterator mBegin;

public:
    Branch(vector<uint8_t>::iterator *it, IAdressMode *adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        // mBegin = begin;
        mPC = it;
    }

    void execute() override
    {
        if (cond())
        {
            int8_t buf = mAdressMode->getValue();
            *mPC += buf;
        }
    }

    virtual bool cond() const = 0;
};

#endif