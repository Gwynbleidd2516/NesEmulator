#ifndef PROCESSOR
#define PROCESSOR

#include <vector>
#include <fstream>
#include "IInstruction.h"
#include "Registers.h"
#include "Flags.h"
#include "Structs.h"
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
};

#endif