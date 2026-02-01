#ifndef IADRESS_MODE
#define IADRESS_MODE

#include <cstdint>
#include <vector>
using namespace std;

class IAdressMode
{
public:
    virtual void code(uint8_t **) = 0;

    virtual void setValue(uint8_t) = 0;

    virtual uint8_t getValue() const = 0;
};

#endif