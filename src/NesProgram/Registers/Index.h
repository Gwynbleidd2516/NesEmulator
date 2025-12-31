#ifndef INDEX
#define INDEX

#include "Integer.h"

class Index : public Integer
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