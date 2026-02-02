#ifndef NES_PROGRAM
#define NES_PROGRAM

#include <string>
#include <vector>
using namespace std;

#include "Processor.h"
#include "Render.h"

class NesProgram
{
private:
    Processor mProcessor;
    Render mRender;

    vector<vector<shared_ptr<IInstruction>>> mInstructions;

public:
    NesProgram();

    void loadFile(string path);

    void step();

    bool isEnd() const;
};

#endif