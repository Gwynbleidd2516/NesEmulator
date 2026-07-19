#ifndef SET_DECIMAL
#define SET_DECIMAL

#include "SetFlag.h"

class SetDecimal : public SetFlag
{
public:
    using SetFlag::SetFlag;

    void execute() override
    {
        mDest->Decimal = mState;
    }
};

#endif