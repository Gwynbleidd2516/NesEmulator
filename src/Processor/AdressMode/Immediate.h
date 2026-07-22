#ifndef IMMEDIATE
#define IMMEDIATE

#include "IAdressMode.h"

class Immediate : public IAdressMode
{
private:
    uint8_t mImm;

public:
    void code(uint8_t **it) override
    {
        (*it)++;
        mImm = **it;
    }

    void setValue(uint8_t val) override
    {
        mImm = val;
    }

    uint8_t getValue() const override
    {
#ifdef DO_LOGS
        Logs::GetInstance().message.append(format("{:x}; ", mImm));
#endif
        return mImm;
    }
};

#endif