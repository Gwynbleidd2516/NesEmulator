#ifndef IINSTRUCTION
#define IINSTRUCTION

#include <vector>
#include "Flags.h"
using namespace std;

class IInstruction
{
public:
    virtual void execute() = 0;

    virtual void code(uint8_t **) = 0;

    // virtual void setFlags(Flags *) {}
};

#endif