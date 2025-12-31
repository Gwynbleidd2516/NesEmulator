#ifndef BCC
#define BCC

#include "HasAdressMode.h"

class Bcc : public HasAdressMode
{

public:
    using HasAdressMode::HasAdressMode;

    void execute() override
    {
    }
};

#endif