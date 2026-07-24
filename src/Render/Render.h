#ifndef RENDER
#define RENDER

#include <fstream>
#include <vector>
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

    OAM *mOAM;
    PPU mPPU;
    Header mHeader;

public:
    Render();

    void setOAM(OAM *oam);

    void loadFromFile(ifstream &file);

    bool isOpen() const;

    void show();

    PPU *getPPU();

    void setHeader(Header h);
};

#endif