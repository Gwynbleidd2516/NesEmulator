#ifndef ZERO_PAGE
#define ZERO_PAGE

#include "IAdressMode.h"

class ZeroPage : public IAdressMode
{
private:
    uint8_t *mMem;
    CPU *mPPU;

public:
    ZeroPage(CPU &ppu)
    {
        mPPU = &ppu;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        mMem = &mPPU->at(**it);
    }

    void setValue(uint8_t val) override
    {
        *mMem = val;
    }

    uint8_t getValue() const override
    {
        return *mMem;
    }
};

class ZeroPageInd : public IAdressMode
{
private:
    uint8_t *mMem;
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
        mMem = &mPPU->at(**it + mReg->getValue());
    }

    void setValue(uint8_t val) override
    {
        *mMem = val;
    }

    uint8_t getValue() const override
    {
        return *mMem;
    }
};

#endif