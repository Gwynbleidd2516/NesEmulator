#ifndef ROL
#define ROL

#include "Shift.h"

class Rol : public Shift
{
public:
    using Shift::Shift;

    void execute() override
    {
        uint8_t old = mAdressMode->getValue();
        uint8_t newCarry = (old & 0x80) ? 1 : 0;
        uint8_t shifted = (old << 1) | (mFlags->Carry ? 1 : 0);
        mFlags->Carry = newCarry;
        mFlags->Zero = (shifted == 0);
        mFlags->Negative = shifted & 0x80;
        mAdressMode->setValue(shifted);
    }
};

#endif