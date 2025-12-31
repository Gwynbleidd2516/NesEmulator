#ifndef BMI
#define BMI

#include "Branch.h"

class Bmi : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return mFlags->Negative;
    }
};

#endif