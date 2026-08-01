#ifndef LSR
#define LSR

#include "Shift.h"

class Lsr : public Shift
{
public:
    using Shift::Shift;

    void execute() override
    {
        uint8_t old = mAdressMode->getValue();
        mFlags->Carry = old & 0x01;
        uint8_t shifted = old >> 1;
        mFlags->Zero = (shifted == 0);
        mFlags->Negative = 0; // всегда 0
        mAdressMode->setValue(shifted);
    }
};

#endif