#ifndef JUMP
#define JUMP

#include "HasAdressMode.h"
#include "StackPointer.h"

class Jump : public HasAdressMode
{
protected:
    vector<uint8_t>::iterator *mPC;
    vector<uint8_t>::iterator mBegin;
    StackPointer *mSP;

public:
    Jump(vector<uint8_t>::iterator begin, shared_ptr<IAdressMode> adressMode, StackPointer *SP) : HasAdressMode(adressMode)
    {
        mBegin = begin;
        mSP = SP;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        mAdressMode->code(it);
        mPC = &it;
    }
};

#endif