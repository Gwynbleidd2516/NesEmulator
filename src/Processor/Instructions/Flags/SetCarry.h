#ifndef SET_CARRY
#define SET_CARRY

#include "SetFlag.h"

class SetCarry : public SetFlag
{
public:
    using SetFlag::SetFlag;

    void execute() override
    {
        mDest->Carry = mState;
    }
};

#endif