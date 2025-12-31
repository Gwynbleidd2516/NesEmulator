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
        int sign = 1;
        if (mFlags->Overflow)
            sign = -1;
        uint8_t ans = mReg->getValue() - sign * mAdressMode->getValue() + !mFlags->Carry;
        int buf = mReg->getValue() - sign * mAdressMode->getValue() + !mFlags->Carry;
        mFlags->Carry = (buf < 0);
        mFlags->Zero = (mAdressMode->getValue() == 0);
        mFlags->Overflow = ((ans ^ mReg->getValue()) & (ans & mAdressMode->getValue()) & 0x80);
        mFlags->Negative = (ans >> 7);
        mReg->setValue(mReg->getValue() - mAdressMode->getValue());
        // *mReg->getResult() -= *mAdressMode->getResult();
    }
};

#endif