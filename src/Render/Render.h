#ifndef RENDER
#define RENDER

#include <fstream>
#include <vector>
#include <atomic>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
using namespace sf;
#include "Cpu.h"
#include "Header.h"

class Render
{
private:
    RenderWindow mWindow;

    Texture mTextueBank1;
    Texture mTextueBank2;

    vector<Sprite> mPatternTable1;
    vector<Sprite> mPatternTable2;

    CPU *mCPU;
    PPU *mPPU;
    Header mHeader;
    
    atomic<bool> *mIsRunning;

public:
    Render();

    void loadFromFile(ifstream &file);

    bool isOpen() const;

    void show();

    void setCPU(CPU *cpu);

    void setPPU(PPU* ppu);

    void setHeader(Header h);

    void setIsRunningAtomic(atomic<bool> *atm);
};

#endif