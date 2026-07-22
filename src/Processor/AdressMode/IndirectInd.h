#ifndef INDIRECT_IND
#define INDIRECT_IND

#include "IAdressMode.h"
#include "Index.h"
#include "Struct16_t.h"

class IndirectX : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mPPU;
    Index *mReg;

public:
    IndirectX(CPU &ppu, Index *reg)
    {
        mPPU = &ppu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        Struct16_t buf = {.h = mPPU->read(**(it) + mReg->getValue()),
                          .l = mPPU->read(**(it) + mReg->getValue() + 1)};

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
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mPPU->at(mLocation)));
#endif
        return mPPU->read(mLocation);
    }
};

class IndirectY : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mPPU;
    Index *mReg;

public:
    IndirectY(CPU &ppu, Index *reg)
    {
        mPPU = &ppu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        Struct16_t buf = {.h = mPPU->read(**(it)),
                          .l = mPPU->read(**(it) + 1)};

        mLocation = buf.raw + mReg->getValue();
#ifdef DO_LOGS
        Logs::GetInstance().adressMode = typeid(*mReg).name() + 6;
#endif
    }

    void setValue(uint8_t val) override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("${:x}; ", mLocation));
#endif
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mPPU->at(mLocation)));
#endif
        return mPPU->read(mLocation);
    }
};

#endif