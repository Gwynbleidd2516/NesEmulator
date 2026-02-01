#ifndef INDIRECT_IND
#define INDIRECT_IND

#include "IAdressMode.h"
#include "Index.h"

class IndirectX : public IAdressMode
{
private:
    uint8_t *mMem;
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
        uint16_t buff = mPPU->at((int)**(it) + (int)mReg->getValue());
        mMem = &mPPU->at(buff);
    }

    void setValue(uint8_t val) override
    {
        *mMem = val;
    }

    uint8_t getValue() const override
    {
        return *mMem;
    }

    // uint8_t *getResult() override
    // {
    //     return mMem;
    // }
};

class IndirectY : public IAdressMode
{
private:
    uint8_t *mMem;
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
        uint16_t buff = mPPU->at((int)**(it));
        mMem = &mPPU->at(buff) + mReg->getValue();
    }

    void setValue(uint8_t val) override
    {
        *mMem = val;
    }

    uint8_t getValue() const override
    {
        return *mMem;
    }

    // uint8_t *getResult() override
    // {
    //     return mMem;
    // }
};

#endif