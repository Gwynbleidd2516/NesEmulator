#ifndef EMPTY
#define EMPTY

#include "IAdressMode.h"

class Empty : public IAdressMode
{
public:
    void code(vector<uint8_t>::iterator &)
    {
    }

    void setValue(uint8_t)
    {
    }

    uint8_t getValue() const
    {
        return 0x0;
    }
};

#endif