#ifndef PROCESSOR
#define PROCESSOR

#include <vector>
#include <fstream>
#include "IInstruction.h"
#include "Registers.h"
#include "Flags.h"
#include "Cpu.h"
using namespace std;

#define NMI_INTERRUPT_LOACTION 0xFFFA
#define RESET_INTERRUPT_LOACTION 0xFFFC
#define IRQ_INTERRUPT_LOACTION 0xFFFE

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

    void reset();

    CPU *getCPU();

    OAM *getOAM();
};

#endif