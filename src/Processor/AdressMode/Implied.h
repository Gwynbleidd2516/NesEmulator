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
#ifdef DO_LOGS
        Logs::GetInstance().adressMode = typeid(*mReg).name() + 6;
#endif
    }

    void setValue(uint8_t val) override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{}({:x}); ", typeid(*mReg).name() + 6, mReg->getValue()));
#endif
        mReg->setValue(val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{}({:x}) -> ", typeid(*mReg).name() + 6, mReg->getValue()));
#endif
        return mReg->getValue();
    }
};

#endif