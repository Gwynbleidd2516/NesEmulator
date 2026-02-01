#ifndef BCC
#define BCC

#include "Branch.h"

class Bcc : public Branch
{
public:
    using Branch::Branch;

    bool cond() const override
    {
        return !mFlags->Carry;
    }
};

#endif