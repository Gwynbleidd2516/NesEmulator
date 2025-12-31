#ifndef ABSOLUTE
#define ABSOLUTE

#include "IAdressMode.h"

class Absolute : public IAdressMode
{
private:
    uint8_t *mMem;
    vector<uint8_t> *mPPU;

public:
    Absolute(vector<uint8_t> &ppu)
    {
        mPPU = &ppu;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        it++;
        mMem = &mPPU->at(from8to16(*it, *(it + 1)));
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

private:
    uint16_t from8to16(uint8_t a1, uint8_t a2)
    {
        uint16_t ans = a2;
        ans *= 256;
        return ans + a1;
    }
};

class AbsoluteInd : public IAdressMode
{
private:
    uint8_t *mMem;
    vector<uint8_t> *mPPU;
    uint8_t *mReg;

public:
    AbsoluteInd(vector<uint8_t> &ppu, uint8_t &reg)
    {
        mPPU = &ppu;
        mReg = &reg;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        it++;

        mMem = &mPPU->at(from8to16(*it, *(it + 1)) + *mReg);
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

private:
    uint16_t from8to16(uint8_t a1, uint8_t a2)
    {
        uint16_t ans = a2;
        ans *= 256;
        return ans + a1;
    }
};

#endif