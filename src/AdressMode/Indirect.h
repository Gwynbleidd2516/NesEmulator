#ifndef INDIRECT
#define INDIRECT

#include "IAdressMode.h"

class Indirect : public IAdressMode
{
private:
    uint8_t *mMem;
    vector<uint8_t> *mPPU;

public:
    Indirect(vector<uint8_t> &ppu)
    {
        mPPU = &ppu;
    }

    void code(vector<uint8_t>::iterator &it) override
    {
        it++;
        mMem = &mPPU->at(mPPU->at(from8to16(*it, *(it + 1))));
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

private:
    uint16_t from8to16(uint8_t a1, uint8_t a2)
    {
        uint16_t ans = a2;
        ans *= 256;
        return ans + a1;
    }
};

#endif