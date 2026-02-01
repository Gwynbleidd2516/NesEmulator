#ifndef STACK_POINTER
#define STACK_POINTER

#include "IRegister.h"
#include <stack>

class StackPointer : public IRegister
{
private:
    std::stack<uint8_t> mStack;

public:
    void setValue(uint8_t val) override
    {
        mStack.pop();
        mStack.push(val);
    }

    uint8_t getValue() const override
    {
        return mStack.top();
    }

    void push(uint8_t val)
    {
        mStack.push(val);
    }

    uint8_t pull()
    {
        uint8_t buf = mStack.top();
        mStack.pop();
        return buf;
    }
};

#endif