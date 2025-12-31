#ifndef INDIRECT
#define INDIRECT

#include "IAdressMode.h"
#include "Index.h"

class IndirectX : public IAdressMode
{
private:
    uint8_t *mMem;
    vector<uint8_t> *mPPU;
    Index *mReg;

public:
    IndirectX(vector<uint8_t> &ppu, Index *reg)
    {
        mPPU = &ppu;
        mReg = reg;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        uint16_t buff = mPPU->at((int)*(it + 1) + (int)mReg->getValue());
        mMem = &mPPU->at(buff);
        it++;
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
    vector<uint8_t> *mPPU;
    Index *mReg;

public:
    IndirectY(vector<uint8_t> &ppu, Index *reg)
    {
        mPPU = &ppu;
        mReg = reg;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        uint16_t buff = mPPU->at((int)*(it + 1));
        mMem = &mPPU->at(buff) + mReg->getValue();
        it++;
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