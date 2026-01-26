#ifndef HAS_ADRESS_MODE
#define HAS_ADRESS_MODE

#include "IInstruction.h"
#include "IAdressMode.h"
#include <memory>
using namespace std;

class HasAdressMode : public IInstruction
{
protected:
    IAdressMode *mAdressMode;

public:
    HasAdressMode(IAdressMode *adressMode)
    {
        mAdressMode = adressMode;
    }

    virtual void code(uint8_t **it) override
    {
        mAdressMode->code(it);
    }

    ~HasAdressMode()
    {
        delete mAdressMode;
    }
};

#endif