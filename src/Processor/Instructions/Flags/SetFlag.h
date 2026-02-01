#ifndef SET_FLAG
#define SET_FLAG

#include "HasFlags.h"

class SetFlag : public IInstruction
{
private:
    bool *mDest;
    bool mState;

public:
    SetFlag(bool *dest, bool state)
    {
        mDest = dest;
        mState = state;
    }

    void execute() override
    {
        *mDest = mState;
    }

    void code(uint8_t **) override
    {
    }
};

#endif