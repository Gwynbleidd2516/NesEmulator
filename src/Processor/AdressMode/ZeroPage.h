#ifndef ZERO_PAGE
#define ZERO_PAGE

#include "IAdressMode.h"

class ZeroPage : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mCPU;

public:
    ZeroPage(CPU *cpu)
    {
        mCPU = cpu;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        mLocation = **it;
#ifdef DO_LOGS
        Logs::GetInstance().adressMode = typeid(*this).name() + 6;
#endif
    }

    void setValue(uint8_t val) override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("${:x}; ", mLocation));
#endif
        mCPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{{${:x}}} {:x}; ", mLocation, mCPU->at(mLocation)));
#endif
        return mCPU->read(mLocation);
    }
};

class ZeroPageInd : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mCPU;
    Index *mReg;

public:
    ZeroPageInd(CPU *cpu, Index *reg)
    {
        mCPU = cpu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        mLocation = mCPU->read(**it + mReg->getValue());
#ifdef DO_LOGS
        Logs::GetInstance().adressMode = typeid(*this).name() + 6;
#endif
    }

    void setValue(uint8_t val) override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("${:x}; ", mLocation));
#endif
        mCPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{{${:x}}} {:x}; ", mLocation, mCPU->at(mLocation)));
#endif
        return mCPU->read(mLocation);
    }
};

#endif