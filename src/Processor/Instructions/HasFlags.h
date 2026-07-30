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
};

#endif