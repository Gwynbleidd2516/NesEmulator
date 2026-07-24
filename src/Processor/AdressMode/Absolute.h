#ifndef ABSOLUTE
#define ABSOLUTE

#include "IAdressMode.h"
#include "Struct16_t.h"

class Absolute : public IAdressMode
{
private:
    uint16_t mLocation;
    CPU *mCPU;
    uint16_t *mJump;

public:
    Absolute(CPU *cpu)
    {
        mCPU = cpu;
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
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("${:x}; ", mLocation, val));
#endif
        mCPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mCPU->at(mLocation)));
#endif
        return mCPU->read(mLocation);
    }

    void setJumpPointer(uint16_t *j)
    {
        mJump = j;
    }
};

class AbsoluteInd : public IAdressMode
{
private:
    uint16_t mLocation;
    CPU *mCPU;
    Index *mReg;

public:
    AbsoluteInd(CPU *cpu, Index *reg)
    {
        mCPU = cpu;
        mReg = reg;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        Struct16_t buf = {.h = **it, .l = *(*it + 1)};
        mLocation = buf.raw + mReg->getValue();
        (*it)++;
#ifdef DO_LOGS
        Logs::GetInstance().adressMode = typeid(*mReg).name() + 6;
#endif
    }

    void setValue(uint8_t val) override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("${:x}; ", mLocation, val));
#endif
        mCPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mCPU->at(mLocation)));
#endif
        return mCPU->read(mLocation);
    }
};

#endif