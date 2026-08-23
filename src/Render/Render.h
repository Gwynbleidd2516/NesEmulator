#ifndef RENDER
#define RENDER

#include <fstream>
#include <vector>
#include <atomic>
using namespace std;
#include "Cpu.h"
#include "Header.h"
#include "NesSprite.h"
#include "Color.h"
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

    const ColorRgb NES_PALETTE[64] = {
        {124, 124, 124}, {0, 0, 252}, {0, 0, 188}, {68, 40, 188}, {148, 0, 132}, {168, 0, 32}, {168, 16, 0}, {136, 20, 0}, {80, 48, 0}, {0, 120, 0}, {0, 104, 0}, {0, 88, 0}, {0, 64, 88}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {188, 188, 188}, {0, 120, 248}, {0, 88, 248}, {104, 68, 252}, {216, 0, 204}, {228, 0, 88}, {248, 56, 0}, {228, 92, 0}, {172, 124, 0}, {0, 184, 0}, {0, 168, 0}, {0, 168, 68}, {0, 136, 136}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {248, 248, 248}, {60, 188, 252}, {104, 136, 252}, {152, 120, 248}, {248, 120, 248}, {248, 88, 152}, {248, 120, 88}, {252, 160, 68}, {248, 184, 0}, {184, 248, 24}, {88, 216, 40}, {88, 248, 152}, {0, 232, 216}, {120, 120, 120}, {0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {164, 228, 252}, {184, 204, 252}, {216, 184, 252}, {252, 184, 252}, {252, 164, 216}, {252, 192, 184}, {252, 208, 160}, {252, 224, 144}, {224, 252, 160}, {184, 248, 184}, {184, 252, 216}, {0, 252, 252}, {216, 216, 216}, {0, 0, 0}, {0, 0, 0}};

public:
    Render();

    void show();

    void setCPU(CPU *cpu);

    void setPPU(PPU *ppu);

    void setHeader(Header h);

    void setIsRunningAtomic(atomic<bool> *atm);

    ~Render();

private:
    ColorRgb nesToRgb(uint8_t nesColor);
};

#endif