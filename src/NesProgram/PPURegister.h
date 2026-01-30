#ifndef PPU_REGISTER
#define PPU_REGISTER

#include <stdint.h>
#include <math.h>

class PPURegister
{
private:
    uint8_t mMemory = 0;

public:
    uint8_t operator=(const uint8_t &mem)
    {
        mMemory = mem;
        return mMemory;
    }

    operator uint8_t() const
    {
        return mMemory;
    }

    bool getBit(uint8_t i) const
    {
        i = i % 8;
        uint8_t mask = pow(2, i);
        uint8_t buf = mMemory & mask;

        return buf >>= i;
    }

    void setBit(uint8_t i, bool val)
    {
        i = i % 8;
        uint8_t mask = pow(2, i);
        if (val)
        {
            mMemory |= mask;
        }
        else
        {
            uint8_t buf = 0xFF - mask;
            mMemory &= buf;
        }
    }
};

#endif