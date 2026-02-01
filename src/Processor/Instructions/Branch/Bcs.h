#ifndef BCS
#define BCS

#include "Branch.h"

class Bcs : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return mFlags->Carry;
    }
};

#endif