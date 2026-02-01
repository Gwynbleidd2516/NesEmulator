#ifndef HAS_FLAGS
#define HAS_FLAGS

#include "IInstruction.h"

class HasFlags
{
protected:
    Flags *mFlags;

public:
    HasFlags(Flags *flags)
    {
        mFlags = flags;
    }

    // void setFlags(Flags *flags) override
    // {
    //     mFlags = flags;
    // }
};

#endif