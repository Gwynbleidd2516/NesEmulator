#ifndef LSR
#define LSR

#include "Shift.h"

class Lsr : public Shift
{
public:
    using Shift::Shift;

    void execute() override
    {
        mAdressMode->setValue(mAdressMode->getValue() >> 1);
        mFlags->Carry = (mAdressMode->getValue() % 2);
        mFlags->Zero = (mAdressMode->getValue() == 0);
        mFlags->Negative = (mAdressMode->getValue() >> 7);
        // *(mAdressMode->getResult()) = *(mAdressMode->getResult()) >> 1;
    }
};

#endif