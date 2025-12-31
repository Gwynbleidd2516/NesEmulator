#ifndef JUMP
#define JUMP

#include "HasAdressMode.h"

class Jump : public HasAdressMode
{
protected:
    vector<uint8_t>::iterator *mPC;
    vector<uint8_t>::iterator mBegin;

public:
    Jump(vector<uint8_t>::iterator begin, shared_ptr<IAdressMode> adressMode) : HasAdressMode(adressMode)
    {
        mBegin = begin;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        mAdressMode->code(it);
        mPC = &it;
    }
};

#endif