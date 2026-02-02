#include "Render.h"
#include <iostream>

Render::Render()
{
    mTextueBank1.resize(Vector2u(16 * 8, 16 * 8));
    mTextueBank2.resize(Vector2u(16 * 8, 16 * 8));
    mWindow.create(VideoMode({800, 600}), "NES emu");
    mPatternTable1;
    mPatternTable2;
}

OAM *Render::getOAM()
{
    return &mOAM[0];
}

void Render::loadFromFile(ifstream &file, size_t size)
{
    DiscreteVal chr[0x2000];
    file.read(reinterpret_cast<char *>(&chr), 0x2000 * size);

    unsigned char mTable1[8 * 16 * 8 * 16];
    unsigned char mTable2[8 * 16 * 8 * 16];
    for (size_t l = 0; l < 0x10; l++)
    {
        for (size_t k = 0; k < 0x8; k++)
        {
            for (size_t j = 0; j < 0x10; j++)
            {
                for (int i = 0; i < 0x8; i++)
                {
                    uint8_t buf = 0;
                    buf = (uint8_t)chr[j * 0x10 + 8 + k + l * 0x100].getBit(0x7 - i) << 1;
                    buf += (uint8_t)chr[j * 0x10 + k + l * 0x100].getBit(0x7 - i);
                    mTable1[i + j * 0x8 + k * 0x10 * 0x8 + l * 0x400] = buf * (float)(255.f / 3.f);
                }
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, mTextueBank1.getNativeHandle());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mTextueBank1.getSize().x, mTextueBank1.getSize().y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, &mTable1);

    for (size_t l = 0; l < 0x10; l++)
    {
        for (size_t k = 0; k < 0x8; k++)
        {
            for (size_t j = 0; j < 0x10; j++)
            {
                for (int i = 0; i < 0x8; i++)
                {
                    uint8_t buf = 0;
                    buf = (uint8_t)chr[0x1000 + j * 0x10 + 8 + k + l * 0x100].getBit(0x7 - i) << 1;
                    buf += (uint8_t)chr[0x1000 + j * 0x10 + k + l * 0x100].getBit(0x7 - i);
                    mTable2[i + j * 0x8 + k * 0x10 * 0x8 + l * 0x400] = buf * (float)(255.f / 3.f);
                }
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, mTextueBank2.getNativeHandle());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mTextueBank2.getSize().x, mTextueBank2.getSize().y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, mTable2);

    for (size_t i = 0; i < 256; i++)
    {
        int a = i / 16;
        Sprite sp(mTextueBank1, IntRect({(i % 16) * 8, (int)(i / 16) * 8}, {8, 8}));
        mPatternTable1.push_back(sp);
    }
    for (size_t i = 0; i < 256; i++)
    {
        int a = i / 16;
        Sprite sp(mTextueBank2, IntRect({(i % 16) * 8, (int)(i / 16) * 8}, {8, 8}));
        mPatternTable2.push_back(sp);
    }
}

bool Render::isOpen() const
{
    return mWindow.isOpen();
}

void Render::show()
{
    while (const std::optional event = mWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            mWindow.close();
    }
    mWindow.clear();

    for (size_t i = 0; i < 64; i++)
    {
        bool table2 = mOAM[i].tile % 2;
        if (table2)
        {
            Sprite buf = mPatternTable1[mOAM[i].tile >> 1];
            buf.setPosition({(float)mOAM[i].x, (float)mOAM[i].y});
            mWindow.draw(buf);
        }
        else
        {
            Sprite buf = mPatternTable2[mOAM[i].tile >> 1];
            buf.setPosition({(float)mOAM[i].x, (float)mOAM[i].y});
            mWindow.draw(buf);
        }
    }

    mWindow.display();
}
