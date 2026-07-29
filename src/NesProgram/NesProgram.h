#ifndef NES_PROGRAM
#define NES_PROGRAM

#include <string>
#include <vector>
#include <thread>
#include <semaphore>
#include <atomic>
using namespace std;

#include "Processor.h"
#include "Render.h"

class NesProgram
{
private:
    Header mHeader;
    CPU mCPU;
    PPU mPPU;
    thread mProcessorThread;
    thread mRenderThread;
    binary_semaphore mLoadSemaphoreProc{0};
    binary_semaphore mLoadSemaphoreRender{0};
    binary_semaphore mLaunchSemaphoreProc{0};
    binary_semaphore mLaunchSemaphoreRender{0};
    atomic<bool> mIsRunningAtomic;

    vector<vector<unique_ptr<IInstruction>>> mInstructions;

public:
    NesProgram();

    void loadFromFile(string path);

    void loadFromStream(istream &stream);

    void step();

    void reset();

private:
    void processThread();

    void renderThread();
};

#endif