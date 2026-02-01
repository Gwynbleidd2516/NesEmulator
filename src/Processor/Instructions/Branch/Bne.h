#ifndef BNE
#define BNE

#include "Branch.h"

class Bne : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return !mFlags->Zero;
    }
};

#endif