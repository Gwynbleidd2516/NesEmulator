#ifndef RENDER
#define RENDER

#include <fstream>
#include <vector>
#include <atomic>
using namespace std;
#include "Cpu.h"
#include "Header.h"

#define GLEW_STATIC
#include "gl/glew.h"
#include "GLFW/glfw3.h"

class Render
{
private:
    GLFWwindow *mWindow;

    CPU *mCPU;
    PPU *mPPU;
    Header mHeader;

    atomic<bool> *mIsRunning;

public:
    Render();

    void loadPattern();

    void show();

    void setCPU(CPU *cpu);

    void setPPU(PPU *ppu);

    void setHeader(Header h);

    void setIsRunningAtomic(atomic<bool> *atm);

    ~Render();
};

#endif