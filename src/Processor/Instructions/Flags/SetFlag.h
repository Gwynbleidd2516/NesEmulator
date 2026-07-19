#ifndef SET_FLAG
#define SET_FLAG

#include "HasFlags.h"

class SetFlag : public IInstruction
{
protected:
    Flags *mDest;
    bool mState;

public:
    SetFlag(Flags *dest, bool state)
    {
        mDest = dest;
        mState = state;
    }

    void code(uint8_t **) override
    {
    }
};

#endif