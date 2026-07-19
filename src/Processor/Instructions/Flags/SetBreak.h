#ifndef SET_BREAK
#define SET_BREAK

#include "SetFlag.h"

class SetBreak : public SetFlag
{
public:
    void execute() override
    {
        mDest->Break = mState;
    }
};

#endif