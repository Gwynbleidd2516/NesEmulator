#ifndef ABSOLUTE
#define ABSOLUTE

#include "IAdressMode.h"
#include "Struct16_t.h"

class Absolute : public IAdressMode
{
private:
    uint16_t mLocation;
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
        Struct16_t buf = {.h = **it, .l = *(*it + 1)};
        if (mJump)
            *mJump = buf.raw;
        mLocation = buf.raw;
        (*it)++;
    }

    void setValue(uint8_t val) override
    {
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
        return mPPU->read(mLocation);
    }

    void setJumpPointer(int *j)
    {
        mJump = j;
    }
};

class AbsoluteInd : public IAdressMode
{
private:
    uint16_t mLocation;
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
        Struct16_t buf = {.h = **it, .l = *(*it + 1)};
        mLocation = buf.raw + mReg->getValue();
        (*it)++;
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