#ifndef BPL
#define BPL

#include "Branch.h"

class Bpl : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return !mFlags->Negative;
    }
};

#endif