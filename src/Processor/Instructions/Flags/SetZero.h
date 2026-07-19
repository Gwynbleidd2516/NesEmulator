#ifndef SET_ZERO
#define SET_ZERO

#include "SetFlag.h"

class SetZero : public SetFlag
{
public:
    using SetFlag::SetFlag;

    void execute() override
    {
        mDest->Zero = mState;
    }
};

#endif