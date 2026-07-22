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
        Logs::GetInstance().message.append(format("{}; ", typeid(*mReg).name() + 6));
        mReg->setValue(val);
    }

    uint8_t getValue() const override
    {
        Logs::GetInstance().message.append(format("{} -> ", typeid(*mReg).name() + 6));
        return mReg->getValue();
    }
};

#endif