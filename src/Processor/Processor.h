#ifndef PROCESSOR
#define PROCESSOR

#include <vector>
#include <fstream>
#include "IInstruction.h"
#include "Registers.h"
#include "Flags.h"
#include "Cpu.h"
using namespace std;

class Processor
{
private:
    vector<vector<shared_ptr<IInstruction>>> mInstructions;
    Registers mRegisters;
    CPU mCPU;

public:
    Processor();

    void loadFromFile(ifstream &file, size_t size);

    void doStep();

    bool eof() const;

private:
    uint16_t from8to16(uint8_t a1, uint8_t a2)
    {
        uint16_t ans = a2;
        ans *= 256;
        return ans + a1;
    }
};

#endif