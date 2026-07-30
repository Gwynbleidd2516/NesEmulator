#ifndef BRANCH
#define BRANCH

#include "HasAdressMode.h"
#include "HasFlags.h"

class Branch : public HasAdressMode, public HasFlags
{
protected:
    uint8_t **mPC;

public:
    Branch(uint8_t **it, shared_ptr<IAdressMode> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
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