#ifndef RENDER
#define RENDER

#include <fstream>
#include <vector>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
using namespace sf;
#include "Cpu.h"

class Render
{
private:
    RenderWindow mWindow;

    Texture mTextueBank1;
    Texture mTextueBank2;

    vector<Sprite> mPatternTable1;
    vector<Sprite> mPatternTable2;

    OAM* mOAM;
    PPU mPPU;

public:
    Render();

    void setOAM(OAM* oam);

    void loadFromFile(ifstream &file, size_t size);

    bool isOpen() const;

    void show();

    PPU* getPPU();
};

#endif