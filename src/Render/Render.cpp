#include "Render.h"
#include <iostream>
#include <DiscreteVal.h>

Render::Render() : mView({256.f / 2.f, 240.f / 2.f}, {256.f, 240.f - 16.0f})
{
    mTextueBank1.resize(Vector2u(16 * 8, 16 * 8));
    mTextueBank2.resize(Vector2u(16 * 8, 16 * 8));
    mWindow.create(VideoMode({256 * 2, (240 - 16) * 2}), "NES emu");
    mWindow.setView(mView);
}

void Render::loadPattern()
{
    DiscreteVal chr[0x2000];
    memcpy(chr, mPPU, 0x2000);

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
                    buf = (uint8_t)chr[0x1000 + j * 0x10 + 8 + k + l * 0x100].getBit(0x7 - i) << 1;
                    buf += (uint8_t)chr[0x1000 + j * 0x10 + k + l * 0x100].getBit(0x7 - i);
                    mTable2[i + j * 0x8 + k * 0x10 * 0x8 + l * 0x400] = buf * (float)(255.f / 3.f);
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

    glBindTexture(GL_TEXTURE_2D, mTextueBank2.getNativeHandle());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mTextueBank2.getSize().x, mTextueBank2.getSize().y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, mTable2);

    for (size_t i = 0; i < 256; i++)
    {
        Sprite sp1(mTextueBank1, IntRect({(i % 16) * 8, (int)(i / 16) * 8}, {8, 8}));
        Sprite sp2(mTextueBank2, IntRect({(i % 16) * 8, (int)(i / 16) * 8}, {8, 8}));
        mPatternTable1.push_back(sp1);
        mPatternTable2.push_back(sp2);
    }
}

bool Render::isOpen() const
{
    return mWindow.isOpen();
}

void Render::show()
{
    while (mWindow.isOpen())
    {
        while (const std::optional event = mWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                mIsRunning->store(false);
                mWindow.close();
            }
        }
        mWindow.clear();

        // vector<Sprite> &table = mPatternTable2;

        // for (size_t i = 0; i < 16; i++)
        // {
        //     for (size_t j = 0; j < 16; j++)
        //     {
        //         Sprite buf = table[i * 16 + j];
        //         buf.setPosition({(float)j * 8.0f, (float)i * 8.0f});
        //         mWindow.draw(buf);
        //     }
        // }
        mView.setCenter({mCPU->ppuscrollx + 256.f / 2.f, mCPU->memoryMap.mPPURegs->ppuscroll + 240.0f / 2.f});
        mWindow.setView(mView);

        vector<Sprite> *patternTable;

        if (mCPU->memoryMap.mPPURegs->ppumask.show_background == 1)
        {
            if (mCPU->memoryMap.mPPURegs->ppuctrl.background_table == 0)
                patternTable = &mPatternTable1;
            else
                patternTable = &mPatternTable2;

            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    Sprite buf = patternTable->at(mPPU->mNametable0[i][j]);
                    buf.setPosition(sf::Vector2f(8.0f * j, 8.0f * i));
                    mWindow.draw(buf);
                    buf = patternTable->at(mPPU->mNametable1[i][j]);
                    buf.setPosition(sf::Vector2f(8.0f * j + 256.0f, 8.0f * i));
                    mWindow.draw(buf);
                    buf = patternTable->at(mPPU->mNametable2[i][j]);
                    buf.setPosition(sf::Vector2f(8.0f * j, 8.0f * i + 240.0f));
                    mWindow.draw(buf);
                    buf = patternTable->at(mPPU->mNametable3[i][j]);
                    buf.setPosition(sf::Vector2f(8.0f * j + 256.0f, 8.0f * i + 240.0f));
                    mWindow.draw(buf);
                }
            }
        }

        if (mCPU->memoryMap.mPPURegs->ppumask.show_sprites == 1)
        {
            if (mCPU->memoryMap.mPPURegs->ppuctrl.sprite_table == 0)
                patternTable = &mPatternTable1;
            else
                patternTable = &mPatternTable2;

            for (size_t i = 0; i < 64; i++)
            {
                Sprite buf = patternTable->at(mCPU->oam[i].tile8x8);
                buf.setPosition({(float)mCPU->oam[i].x,
                                 (float)mCPU->oam[i].y});
                buf.setScale(Vector2f(-1.0f + !mCPU->memoryMap.mMirror->oam[i].flipHorizontally * 2.0f,
                                      -1.0f + !mCPU->memoryMap.mMirror->oam[i].flipVertically * 2.0f));
                mWindow.draw(buf);
            }
        }
        mWindow.display();
    }
}

void Render::setCPU(CPU *cpu)
{
    mCPU = cpu;
}

void Render::setPPU(PPU *ppu)
{
    mPPU = ppu;
}

void Render::setHeader(Header h)
{
    mHeader = h;
}

void Render::setIsRunningAtomic(atomic<bool> *atm)
{
    mIsRunning = atm;
}
