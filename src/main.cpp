#include "NesProgram.h"
#include <iostream>

int main(int argv, char **argc)
{
    try
    {
        if (argv == 2)
        {
            NesProgram pr;
            pr.loadFromFile(argc[1]);
            pr.reset();
            pr.step();
        }
        spdlog::shutdown();
    }
    catch (const std::exception &e)
    {
        spdlog::shutdown();
        spdlog::error(e.what());
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}