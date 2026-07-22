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
        Logs::GetInstance().adressMode = typeid(*mReg).name() + 6;
    }

    void setValue(uint8_t val) override
    {
        Logs::GetInstance().message.append(format("{}; ", typeid(*mReg).name() + 6));
        mReg->setValue(val);
    }

    uint8_t getValue() const override
    {
        Logs::GetInstance().message.append(format("{}({:x}) -> ", typeid(*mReg).name() + 6, mReg->getValue()));
        return mReg->getValue();
    }
};

#endif