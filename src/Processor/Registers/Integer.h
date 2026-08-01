#ifndef INTEGER
#define INTEGER

#include "IRegister.h"
#include "Flags.h"

class Integer : public IRegister
{
protected:
    uint8_t mValue = 0x0;
    Flags *mFlags;

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