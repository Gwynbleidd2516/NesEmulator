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

    void code(uint8_t **)
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
};

#endif