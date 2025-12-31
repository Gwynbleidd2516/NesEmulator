#ifndef JMP
#define JMP

#include "Jump.h"

class Jmp : public Jump
{
public:
    using Jump::Jump;

    // Cmp(shared_ptr<Implied> reg, shared_ptr<IAdressMode> adressMode) : HasAdressMode(adressMode)
    // {
    //     mReg = reg;
    // }

    void execute() override
    {
        *mPC = mBegin + mAdressMode->getValue();
    }
};

#endif