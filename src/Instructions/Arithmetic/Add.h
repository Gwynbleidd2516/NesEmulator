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
        int sign = 1;
        if (mFlags->Overflow)
            sign = -1;
        uint8_t ans = mReg->getValue() + sign * mAdressMode->getValue() + mFlags->Carry;
        int buf = mReg->getValue() + sign * mAdressMode->getValue() + mFlags->Carry;
        mFlags->Carry = (buf > 0xFF);
        mFlags->Zero = (mAdressMode->getValue() == 0);
        mFlags->Overflow = ((ans ^ mReg->getValue()) & (ans & mAdressMode->getValue()) & 0x80);
        mFlags->Negative = (ans >> 7);
        mReg->setValue(mAdressMode->getValue());
        // mReg->setValue(mReg->getValue() + mAdressMode->getValue());
        // *mReg->getResult() += *mAdressMode->getResult();
    }
};

#endif