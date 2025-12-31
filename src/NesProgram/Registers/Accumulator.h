#ifndef ACCUMULATOR
#define ACCUMULATOR

#include "Integer.h"

class Accumulator : public Integer
{
private:
    uint8_t mValue;

public:
    void setValue(uint8_t val) override
    {
        mValue = val;
    }

    uint8_t getValue() const override
    {
        return mValue;
    }
};

#endif