#ifndef INDIRECT
#define INDIRECT

#include "IAdressMode.h"

class Indirect : public IAdressMode
{
private:
    uint16_t mLocation;
    CPU *mPPU;
    uint16_t *mJump;

public:
    Indirect(CPU &ppu)
    {
        mPPU = &ppu;
    }

    void code(uint8_t **it) override
    {
        (*it)++;
        Struct16_t buf = {.h = **it, .l = *(*it + 1)};
        *mJump = buf.raw;
        Struct16_t buf2 = {.h = mPPU->read(buf.raw), .l = mPPU->read(buf.raw + 1)};
        mLocation = buf2.raw;
        (*it)++;
    }

    void setValue(uint8_t val) override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("${:x}; ", mLocation, val));
#endif
        mPPU->write(mLocation, val);
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{{${:x}}} {:x} -> ", mLocation, mPPU->at(mLocation)));
#endif
        return mPPU->read(mLocation);
    }

    void setJumpPointer(uint16_t *j)
    {
        mJump = j;
    }
};

#endif