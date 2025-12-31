#ifndef BEQ
#define BEQ

#include "Branch.h"

class Beq : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return mFlags->Zero;
    }
};

#endif