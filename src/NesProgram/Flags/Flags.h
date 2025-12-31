#ifndef FLAGS
#define FLAGS

#include <cstring>
#include <stdint.h>

#pragma pack(push, 1)
struct Flags
{
    bool Negative;
    bool Overflow;
    bool Break;
    bool Decimal;
    bool Interrupt;
    bool Zero;
    bool Carry;

public:
    Flags()
    {
        memset(this, false, sizeof(Flags));
    }

    operator uint8_t() const
    {
        uint8_t ans = 0;

        int cap = 0;

        for (bool *i = (bool *)this; cap < sizeof(Flags); i += sizeof(bool))
        {
            uint8_t buf = *i << cap;
            ans |= buf;
            cap++;
        }

        // memcpy(&ans, this, sizeof(Flags));

        return ans;
    }

    Flags operator=(uint8_t num)
    {
        bool *i = (bool *)this;
        while (num)
        {
            *i = num % 2;
            i++;
            num /= 2;
        }

        return *this;
    }
};
#pragma pack(pop)

#endif