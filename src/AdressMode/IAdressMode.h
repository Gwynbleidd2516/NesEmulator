#ifndef IADRESS_MODE
#define IADRESS_MODE

#include <cstdint>
#include <vector>
using namespace std;

class IAdressMode
{
public:
    virtual void code(vector<uint8_t>::iterator &) = 0;

    // virtual uint8_t *getResult() = 0;

    virtual void setValue(uint8_t) = 0;

    virtual uint8_t getValue() const = 0;

    // virtual uint8_t operator=(uint8_t val) = 0;

    // virtual uint8_t operator+=(uint8_t val) = 0;

    // virtual uint8_t operator-=(uint8_t val) = 0;

    // virtual uint8_t operator++() = 0;

    // virtual uint8_t operator--() = 0;

    // virtual uint8_t operator<<=(uint8_t val) = 0;

    // virtual uint8_t operator>>=(uint8_t val) = 0;

    // virtual uint8_t operator&=(uint8_t val) = 0;

    // virtual uint8_t operator|=(uint8_t val) = 0;

    // virtual uint8_t operator^=(uint8_t val) = 0;

    // virtual uint8_t operator&(uint8_t val) = 0;
};

#endif