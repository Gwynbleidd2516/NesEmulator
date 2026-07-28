#ifndef PROCESSOR
#define PROCESSOR

#include <vector>
#include <fstream>
#include "IInstruction.h"
#include "Registers.h"
#include "Flags.h"
#include "Cpu.h"
#include "Header.h"
using namespace std;
#include <SFML/System.hpp>

class Processor
{
private:
    vector<vector<shared_ptr<IInstruction>>> mInstructions;
    Registers mRegisters;
    Header mHeader;
    CPU *mCPU;
    sf::Clock mNmiClock;
    sf::Clock mWarpNmiClock;

public:
    Processor();

    void launch();

    bool eof() const;

    void reset();

    void nmi();

    void setCPU(CPU *cpu);

    void setPPU(PPU *ppu);

    void setHeader(Header h);
};

#endif