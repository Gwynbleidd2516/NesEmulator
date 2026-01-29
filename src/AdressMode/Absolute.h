#ifndef ABSOLUTE
#define ABSOLUTE

#include "IAdressMode.h"

class Absolute : public IAdressMode
{
private:
    uint8_t *mMem;
    CPU *mPPU;
    int *mJump;

public:
    Absolute(CPU &ppu)
    {
        mPPU = &ppu;
        mJump = nullptr;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        if (mJump)
            *mJump = from8to16(**it, *(*it + 1));
        mMem = &(mPPU->at(from8to16(**it, *(*it + 1))));
        (*it)++;
    }

    void setValue(uint8_t val) override
    {
        *mMem = val;
    }

    uint8_t getValue() const override
    {
        return *mMem;
    }

    void setJumpPointer(int *j)
    {
        mJump = j;
    }

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
    CPU *mPPU;
    Index *mReg;

public:
    AbsoluteInd(CPU &ppu, Index *reg)
    {
        mPPU = &ppu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;

        mMem = &mPPU->at(from8to16(**it, *(*it + 1)) + mReg->getValue());
        (*it)++;
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