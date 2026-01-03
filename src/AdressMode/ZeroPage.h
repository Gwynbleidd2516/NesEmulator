#ifndef ZERO_PAGE
#define ZERO_PAGE

#include "IAdressMode.h"

class ZeroPage : public IAdressMode
{
private:
    uint8_t *mMem;
    vector<uint8_t> *mPPU;

public:
    ZeroPage(vector<uint8_t> &ppu)
    {
        mPPU = &ppu;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        it++;
        mMem = &mPPU->at(*it);
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

class ZeroPageInd : public IAdressMode
{
private:
    uint8_t *mMem;
    vector<uint8_t> *mPPU;
    Index *mReg;

public:
    ZeroPageInd(vector<uint8_t> &ppu, Index *reg)
    {
        mPPU = &ppu;
        mReg = reg;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        it++;
        mMem = &mPPU->at(*it + mReg->getValue());
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