#ifndef SET_OVERFLOW
#define SET_OVERFLOW

#include "SetFlag.h"

class SetOverflow : public SetFlag
{
public:
    using SetFlag::SetFlag;

    void execute() override
    {
        mDest->Overflow = mState;
    }
};

#endif