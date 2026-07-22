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
    }

    void setValue(uint8_t val) override
    {
        Logs::GetInstance().message.append(format("${:x}; ", mLocation));
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mPPU->at(mLocation)));
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
    }

    void setValue(uint8_t val) override
    {
        Logs::GetInstance().message.append(format("${:x}; ", mLocation));
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mPPU->at(mLocation)));
        return mPPU->read(mLocation);
    }
};

#endif