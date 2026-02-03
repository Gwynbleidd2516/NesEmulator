#ifndef RENDER
#define RENDER

#include <fstream>
#include <vector>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
using namespace sf;

#include "DiscreteVal.h"
#include "Structs.h"

class Render
{
private:
    RenderWindow mWindow;

    Texture mTextueBank1;
    Texture mTextueBank2;

    vector<Sprite> mPatternTable1;
    vector<Sprite> mPatternTable2;

    OAM mOAM[64];

public:
    Render();

    OAM *getOAM();

    void loadFromFile(ifstream &file, size_t size);

    bool isOpen() const;

    void show();
};

#endif