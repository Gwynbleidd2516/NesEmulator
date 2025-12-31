#include "NesProgram.h"
#include <iostream>
using namespace std;

int main()
{
    uint8_t a = 5;
    uint8_t b = -8;
    uint8_t ans = 1 ^ 0;
    // int b = a >> 7;
    bool c = ((ans ^ a) & (ans & b) & 0x80);

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