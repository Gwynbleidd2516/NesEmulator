#ifndef ASL
#define ASL

#include "Shift.h"

class Asl : public Shift
{
public:
    using Shift::Shift;

    void execute() override
    {
        uint8_t old = mAdressMode->getValue();
        uint8_t shifted = old << 1;
        mFlags->Carry = (old & 0x80) ? 1 : 0;
        mFlags->Zero = (shifted == 0);
        mFlags->Negative = (shifted & 0x80);
        mAdressMode->setValue(shifted);
    }
};

#endif