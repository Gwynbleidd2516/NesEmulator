#ifndef SET_INTERRUPT
#define SET_INTERRUPT

#include "SetFlag.h"

class SetInterrupt : public SetFlag
{
public:
    using SetFlag::SetFlag;

    void execute() override
    {
        mDest->Interrupt = mState;
    }
};

#endif