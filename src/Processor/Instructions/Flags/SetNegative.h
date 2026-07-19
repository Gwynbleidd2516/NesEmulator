#ifndef SET_NEGATIVE
#define SET_NEGATIVE

#include "SetFlag.h"

class SetNegative : public SetFlag
{
public:
    using SetFlag::SetFlag;

    void execute() override
    {
        mDest->Negative = mState;
    }
};

#endif