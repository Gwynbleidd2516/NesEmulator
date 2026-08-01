#ifndef ADD
#define ADD

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Add : public HasAdressMode, public HasFlags
{
private:
    shared_ptr<Implied> mReg;

public:
    Add(shared_ptr<Implied> reg, shared_ptr<IAdressMode> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mReg = reg;
    }

    void execute() override
    {
        uint8_t operand = mAdressMode->getValue();
        uint8_t carry = mFlags->Carry ? 1 : 0;
        uint16_t result = mReg->getValue() + operand + carry;
        mFlags->Carry = (result > 0xFF);
        mFlags->Zero = ((result & 0xFF) == 0);
        mFlags->Overflow = ((~(mReg->getValue() ^ operand) & (mReg->getValue() ^ (uint8_t)result) & 0x80) != 0);
        mFlags->Negative = (result & 0x80);
        mReg->setValue(result & 0xFF);
    }
};

#endif