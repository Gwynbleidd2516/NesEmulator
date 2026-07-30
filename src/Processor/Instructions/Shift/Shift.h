#ifndef SHIFT
#define SHIFT

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Shift : public HasAdressMode, public HasFlags
{
public:
    Shift(shared_ptr<IAdressMode> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
    }
};

#endif