#ifndef BVS
#define BVS

#include "Branch.h"

class Bvs : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return mFlags->Overflow;
    }
};

#endif