#ifndef IMMEDIATE
#define IMMEDIATE

#include "IAdressMode.h"

class Immediate : public IAdressMode
{
private:
    uint8_t mImm;

public:
    void code(vector<uint8_t>::iterator &it) override
    {
        it++;
        mImm = *it;
    }

    void setValue(uint8_t val) override
    {
        mImm = val;
    }

    uint8_t getValue() const override
    {
        return mImm;
    }

    // uint8_t *getResult() override
    // {
    //     return &mImm;
    // }
};

#endif