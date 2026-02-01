#ifndef JMP
#define JMP

#include "Jump.h"

class Jmp : public Jump
{
public:
    using Jump::Jump;

    void execute() override
    {
        *mPC = (uint8_t *)mBegin + mJump - 1;
    }
};

#endif