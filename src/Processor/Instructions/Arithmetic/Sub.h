#ifndef SUB
#define SUB

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Sub : public HasAdressMode, public HasFlags
{
private:
    shared_ptr<Implied> mReg;

public:
    Sub(shared_ptr<Implied> reg, shared_ptr<IAdressMode> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mReg = reg;
    }

    void execute() override
    {
        uint8_t operand = mAdressMode->getValue();
        uint8_t borrow = mFlags->Carry ? 0 : 1; // если Carry=1, то вычитание без заёма
        int16_t result = mReg->getValue() - operand - borrow;
        mFlags->Carry = (result >= 0);
        mFlags->Zero = ((result & 0xFF) == 0);
        mFlags->Overflow = (((mReg->getValue() ^ operand) & 0x80) && ((mReg->getValue() ^ (uint8_t)result) & 0x80));
        mFlags->Negative = (result & 0x80);
        mReg->setValue(result & 0xFF);
    }
};

#endif