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

#define NMI_INTERRUPT_LOACTION 0xFA
#define RESET_INTERRUPT_LOACTION 0xFC
#define IRQ_INTERRUPT_LOACTION 0xFE

class Processor
{
private:
    vector<vector<shared_ptr<IInstruction>>> mInstructions;
    Registers mRegisters;
    Header mHeader;
    CPU mCPU;
    sf::Clock mNmiClock;
    sf::Clock mWarpNmiClock;

public:
    Processor();

    void loadFromFile(ifstream &file);

    void doStep();

    bool eof() const;

    void reset();

    void nmi();

    CPU *getCPU();

    OAM *getOAM();

    void setHeader(Header h);
};

#endif