#ifndef INDIRECT_IND
#define INDIRECT_IND

#include "IAdressMode.h"
#include "Index.h"
#include "Struct16_t.h"

class IndirectX : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mCPU;
    Index *mReg;

public:
    IndirectX(CPU *cpu, Index *reg)
    {
        mCPU = cpu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        Struct16_t buf = {.h = mCPU->read((uint8_t)(**(it) + mReg->getValue())),
                          .l = mCPU->read((uint8_t)(**(it) + mReg->getValue() + 1))};

        mLocation = buf.raw;
#ifdef DO_LOGS
        Logs::GetInstance().adressMode = typeid(*mReg).name() + 6;
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
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mCPU->at(mLocation)));
#endif
        return mCPU->read(mLocation);
    }
};

class IndirectY : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mCPU;
    Index *mReg;

public:
    IndirectY(CPU *cpu, Index *reg)
    {
        mCPU = cpu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        Struct16_t buf = {.h = mCPU->read(**(it)),
                          .l = mCPU->read(**(it) + 1)};

        mLocation = (uint8_t)(buf.raw + mReg->getValue());
#ifdef DO_LOGS
        Logs::GetInstance().adressMode = typeid(*mReg).name() + 6;
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
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mCPU->at(mLocation)));
#endif
        return mCPU->read(mLocation);
    }
};

#endif