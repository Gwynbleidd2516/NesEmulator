#ifndef IMPLIED
#define IMPLIED

#include "IAdressMode.h"
#include "Integer.h"

class Implied : public IAdressMode
{
private:
    IRegister *mReg;

public:
    Implied(Integer *src)
    {
        mReg = src;
    }

    void code(vector<uint8_t>::iterator &)
    {
    }

    void setValue(uint8_t val) override
    {
        mReg->setValue(val);
    }

    uint8_t getValue() const override
    {
        return mReg->getValue();
    }

    // uint8_t operator=(uint8_t val)
    // {
    //     mFlags->Zero = (val == 0);
    //     mFlags->Negative = (val >> 7);
    //     mValue = val;
    //     return mValue;
    // }

    // uint8_t operator+=(uint8_t val)
    // {
    //     int sign = 1;
    //     if (mFlags->Overflow)
    //         sign = -1;
    //     uint8_t ans = mValue + sign * val + mFlags->Carry;
    //     int buf = mValue + sign * val + mFlags->Carry;
    //     mFlags->Carry = (buf > 0xFF);
    //     mFlags->Zero = (val == 0);
    //     mFlags->Overflow = ((ans ^ mValue) & (ans & val) & 0x80);
    //     mFlags->Negative = (ans >> 7);
    //     mValue = ans;
    //     return mValue;
    // }

    // uint8_t operator-=(uint8_t val)
    // {
    //     int sign = 1;
    //     if (mFlags->Overflow)
    //         sign = -1;
    //     uint8_t ans = mValue - sign * val + !mFlags->Carry;
    //     int buf = mValue - sign * val + !mFlags->Carry;
    //     mFlags->Carry = (buf < 0);
    //     mFlags->Zero = (val == 0);
    //     mFlags->Overflow = ((ans ^ mValue) & (ans & !val) & 0x80);
    //     mFlags->Negative = (ans >> 7);
    //     mValue = ans;
    //     return mValue;
    // }

    // uint8_t operator++()
    // {
    //     mValue++;
    //     mFlags->Zero = (mValue == 0);
    //     mFlags->Negative = (mValue >> 7);
    //     return mValue;
    // }

    // uint8_t operator--()
    // {
    //     mValue--;
    //     mFlags->Zero = (mValue == 0);
    //     mFlags->Negative = (mValue >> 7);
    //     return mValue;
    // }

    // uint8_t operator<<=(uint8_t val)
    // {
    //     val = mValue << val;
    //     mFlags->Carry = (val >> 7);
    //     mFlags->Zero = (val == 0);
    //     mFlags->Negative = (val >> 7);
    //     mValue = val;
    //     return mValue;
    // }

    // uint8_t operator>>=(uint8_t val)
    // {
    //     val = mValue << val;
    //     mFlags->Carry = (val % 2);
    //     mFlags->Zero = (val == 0);
    //     mFlags->Negative = (val >> 7);
    //     mValue = val;
    //     return mValue;
    // }

    // uint8_t operator&=(uint8_t val)
    // {
    //     mValue &= val;
    //     mFlags->Zero = (mValue == 0);
    //     mFlags->Negative = (mValue >> 7);
    //     return mValue;
    // }

    // uint8_t operator|=(uint8_t val)
    // {
    //     mValue |= val;
    //     mFlags->Zero = (mValue == 0);
    //     mFlags->Negative = (mValue >> 7);
    //     return mValue;
    // }

    // uint8_t operator^=(uint8_t val)
    // {
    //     mValue ^= val;
    //     mFlags->Zero = (mValue == 0);
    //     mFlags->Negative = (mValue >> 7);
    //     return mValue;
    // }

    // uint8_t operator&(uint8_t val)
    // {
    //     uint8_t res = mValue & val;
    //     mFlags->Zero = (res == 0);
    //     mFlags->Overflow = ((res >> 6) % 2);
    //     mFlags->Negative = (res >> 7);
    //     return res;
    // }
};

#endif