#ifndef IREGISTER
#define IREGISTER

#include <cstdint>

class IRegister
{
public:
    virtual void setValue(uint8_t) = 0;

    virtual uint8_t getValue() const = 0;
};

#endif