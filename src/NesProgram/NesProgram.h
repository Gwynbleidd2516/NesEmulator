#ifndef NES_PROGRAM
#define NES_PROGRAM

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "IInstruction.h"
// #include "IAdressMode.h"
#include <memory>
using namespace std;

#include "Registers.h"
#include "Flags.h"
#include "Structs.h"

#define FUNC_ARGS vector<uint8_t>::iterator &it, Registers &reg, vector<uint8_t> &ppu

class NesProgram
{
private:
    // Header mHeader;
    // vector<uint8_t> mCode;
    // vector<uint8_t> mData;
    NesFile mNesFile;
    CPU mCPU;
    vector<uint8_t> mPPU;
    Registers mRegisters;

    vector<uint8_t>::iterator mCodeIt;
    vector<vector<shared_ptr<IInstruction>>> mInstructions;

public:
    NesProgram();

    void loadFile(string path);

    void step();

    bool eof() const;

    // private:
    // static map<uint8_t, function<void(FUNC_ARGS)>> mComands;
};

#endif