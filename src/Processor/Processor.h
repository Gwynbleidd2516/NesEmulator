#ifndef PROCESSOR
#define PROCESSOR

#include <vector>
#include <atomic>
#include <chrono>
#include "IInstruction.h"
#include "Registers.h"
#include "Flags.h"
#include "Cpu.h"
#include "Header.h"
using namespace std;

using Clock = chrono::system_clock;

class Processor
{
private:
    vector<vector<shared_ptr<IInstruction>>> mInstructions;
    Registers mRegisters;
    Header mHeader;
    CPU *mCPU;
    Clock::time_point mNmiClock;
    atomic<bool> *mIsRunning;

public:
    Processor();

    void launch();

    bool eof() const;

    void reset();

    void nmi();

    void setCPU(CPU *cpu);

    void setPPU(PPU *ppu);

    void setHeader(Header h);

    void setIsRunningAtomic(atomic<bool> *atm);
};

#endif