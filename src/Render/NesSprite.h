#ifndef NES_SPRITE
#define NES_SPRITE

#include "Pattern.h"
#include "gl/glew.h"
#include <string>

class NesSprite
{
private:
    static int mInstanceCapasity;
    static unsigned int mVAO, mVBO, mEBO;
    static unsigned int mShaderProgram;
    double mPosX = 0.0;
    double mPosY = 0.0;
    Pattern mPattern;
    bool mFlipVertically = false;
    bool mFlipHorizontally = false;
    uint8_t mScrollX = 0x0;
    uint8_t mScrollY = 0x0;

public:
    NesSprite();

    void draw();

    void setTexture(Pattern pt);

    void setPosition(double x, double y) noexcept;

    void setFlips(bool v, bool h) noexcept;

    void setScroll(uint8_t x, uint8_t y) noexcept;

    double getX() const;

    double getY() const;

    ~NesSprite();

private:
    static void init();

    static void createShader();

    static void destroy();

    static int findLocation(std::string name);

    static float *getProjectionMat(float left, float right, float bottom, float top, float near, float far);
};

#endif