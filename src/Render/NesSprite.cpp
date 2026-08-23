#include "NesSprite.h"
#include <exception>
#include <iostream>
#include <format>

int NesSprite::mInstanceCapasity = 0;
unsigned int NesSprite::mVAO;
unsigned int NesSprite::mVBO;
unsigned int NesSprite::mEBO;
unsigned int NesSprite::mShaderProgram;

NesSprite::NesSprite()
{
    if (mInstanceCapasity == 0)
    {
        init();
        createShader();
    }
    mInstanceCapasity++;
}

void NesSprite::draw()
{
    glUniform3fv(findLocation("aPallete"), 4, &mPalete[0].r);
    glUniform2f(findLocation("aPos"), mPosX, mPosY);
    glUniform2ui(findLocation("aLayer1"), mPattern.layer11, mPattern.layer12);
    glUniform2ui(findLocation("aLayer2"), mPattern.layer21, mPattern.layer22);
    glUniform1i(findLocation("aFlipVertically"), mFlipVertically);
    glUniform1i(findLocation("aFlipHorizontally"), mFlipHorizontally);
    glUniform2i(findLocation("aScroll"), mScrollX, mScrollY);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void NesSprite::setTexture(Pattern pt)
{
    mPattern = pt;
}

void NesSprite::setPosition(double x, double y) noexcept
{
    mPosX = x;
    mPosY = y;
}

void NesSprite::setFlips(bool v, bool h) noexcept
{
    mFlipVertically = v;
    mFlipHorizontally = h;
}

void NesSprite::setScroll(uint8_t x, uint8_t y) noexcept
{
    mScrollX = x;
    mScrollY = y;
}

void NesSprite::setPallete(ColorRgb color[4])
{
    for (size_t i = 0; i < 4; i++)
    {
        mPalete[i] = color[i];
    }
}

double NesSprite::getX() const
{
    return mPosX;
}

double NesSprite::getY() const
{
    return mPosY;
}

NesSprite::~NesSprite()
{
    mInstanceCapasity--;
    if (mInstanceCapasity == 0)
    {
        destroy();
    }
}

void NesSprite::init()
{
    float vertices[] = {
        4.0f,
        4.0f, // Top Right
        4.0f,
        -4.0, // Bottom Right
        -4.0,
        -4.0, // Bottom Left
        -4.0,
        4.0f, // Top Left
    };
    unsigned int indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void NesSprite::createShader()
{
    const char *vertexShaderSource =
        R"glsl(
    #version 460 core
    layout (location = 0) in vec2 aVert;
    uniform mat4 aProj;
    uniform vec2 aPos;
    uniform bool aFlipVertically;
    uniform bool aFlipHorizontally;
    uniform ivec2 aScroll;
    out vec2 mVert;

    void main()
    {
        vec4 shift = vec4(4.0, 4.0, 0.0, 0.0);
        gl_Position = (vec4(aVert, 0.0, 1.0) + shift + vec4(aPos,0.0,0.0) - vec4(aScroll,0.0,0.0)) * aProj;
        mVert = aVert;
        if (aFlipVertically)
            mVert.y*=-1.0;
        if (aFlipHorizontally)
            mVert.x*=-1.0;
        mVert+=shift.xy;
    }
)glsl";

    const char *fragmentShaderSource =
        R"glsl(
    #version 460 core

    in vec2 mVert;
    uniform uvec2 aLayer1;
    uniform uvec2 aLayer2;
    uniform vec3 aPallete[4];
    out vec4 FragColor;

    uint getDepth()
    {
        ivec2 pos = ivec2(mVert);
        uvec2 l;
        if(pos.y<4)
        {
            l.x = aLayer1.x;
            l.y = aLayer2.x;
        }
        else
        {
            l.x = aLayer1.y;
            l.y = aLayer2.y;
        }
        pos.y = pos.y % 4;
        l.x >>= uint(pos.y * 8);
        l.y >>= uint(pos.y * 8);
    
        uint bitX = 7 - uint(pos.x);
        uint bit1 = (l.x >> bitX) & 1u;
        uint bit2 = (l.y >> bitX) & 1u;

        uint num = bit2 * 2u + bit1;
        return num;
    }

    void main() 
    {
        uint d = getDepth();
        if (d != 0)
            FragColor = vec4(aPallete[d], 1.0);
        else
            FragColor = vec4(0.0);
    }
)glsl";

    GLint success;
    GLchar infoLog[512];

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        throw std::runtime_error(std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED {}", infoLog));
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        throw std::runtime_error(std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED {}", infoLog));
    }

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(mShaderProgram);
    float *mat = getProjectionMat(0.0, 256, 240 - 8, 8.0, -1, 1);
    glUniformMatrix4fv(findLocation("aProj"), 1, GL_FALSE, mat);
    delete[] mat;
}

void NesSprite::destroy()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteProgram(mShaderProgram);
}

int NesSprite::findLocation(std::string name)
{
    GLint loc = glGetUniformLocation(mShaderProgram, name.c_str());
    if (loc == -1)
    {
        throw std::runtime_error(std::format("Uniform {} is not found!!!", name).c_str());
    }

    return loc;
}

float *NesSprite::getProjectionMat(float left, float right, float bottom, float top, float near, float far)
{
    return new float[16]{2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
                         0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
                         0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near),
                         0.0f, 0.0f, 0.0f, 1.0f};
}
