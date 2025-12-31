#include "NesProgram.h"
#include <iostream>
using namespace std;

int main()
{
    try
    {
        NesProgram pr;
        pr.loadFile("example.nes");
        // pr.loadFile("Super Mario Bros.nes");

        while (!pr.eof())
        {
            pr.step();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}