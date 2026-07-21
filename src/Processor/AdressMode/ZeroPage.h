#ifndef ZERO_PAGE
#define ZERO_PAGE

#include "IAdressMode.h"

class ZeroPage : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mPPU;

public:
    ZeroPage(CPU &ppu)
    {
        mPPU = &ppu;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        mLocation = **it;
    }

    void setValue(uint8_t val) override
    {
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
        return mPPU->read(mLocation);
    }
};

class ZeroPageInd : public IAdressMode
{
private:
    uint8_t mLocation;
    CPU *mPPU;
    Index *mReg;

public:
    ZeroPageInd(CPU &ppu, Index *reg)
    {
        mPPU = &ppu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        mLocation = mPPU->read(**it + mReg->getValue());
    }

    void setValue(uint8_t val) override
    {
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
        return mPPU->read(mLocation);
    }
};

#endif