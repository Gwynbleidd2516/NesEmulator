#ifndef BVC
#define BVC

#include "Branch.h"

class Bvc : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return !mFlags->Overflow;
    }
};

#endif