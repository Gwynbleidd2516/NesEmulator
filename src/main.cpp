#include "NesProgram.h"
#include <iostream>

int main(int argv, char **argc)
{
    try
    {
        if (argv == 2)
        {
            NesProgram pr;
            pr.loadFile(argc[1]);

            while (!pr.isEnd())
            {
                pr.step();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}