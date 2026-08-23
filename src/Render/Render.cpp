#include "Render.h"
#include <iostream>
#include <DiscreteVal.h>

Render::Render()
{
    if (!glfwInit())
    {
        throw runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    mWindow = glfwCreateWindow(256 * 3, (240 - 16) * 3, "NES emualtor", nullptr, nullptr);
    if (!mWindow)
    {
        glfwTerminate();
        throw runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(mWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW");
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render::show()
{
    NesSprite sprite;
    NesSprite background;
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Pattern *patternTable;

        if (mCPU->memoryMap.mPPURegs->ppumask.show_background == 1)
        {
            patternTable = (mCPU->memoryMap.mPPURegs->ppuctrl.background_table == 0)
                               ? mPPU->mPatternTable0
                               : mPPU->mPatternTable1;

            uint8_t nametableIndex = mCPU->memoryMap.mPPURegs->ppuctrl.nametable;

            background.setScroll(mCPU->ppuscrollx, mCPU->memoryMap.mPPURegs->ppuscroll);
            uint8_t (*nametable)[32] = nullptr;
            AttributeTable *attributeTable = nullptr;
            switch (nametableIndex)
            {
            case 0:
                nametable = mPPU->mNametable0;
                attributeTable = mPPU->mAttributeTable0;
                break;
            case 1:
                nametable = mPPU->mNametable1;
                attributeTable = mPPU->mAttributeTable1;
                break;
            case 2:
                nametable = mPPU->mNametable2;
                attributeTable = mPPU->mAttributeTable2;
                break;
            case 3:
                nametable = mPPU->mNametable3;
                attributeTable = mPPU->mAttributeTable3;
                break;
            }

            for (int tileY = 0; tileY < 30; ++tileY)
            {
                for (int tileX = 0; tileX < 32; ++tileX)
                {
                    // Индекс тайла в намтейбле
                    uint8_t tileIndex = nametable[tileY][tileX];

                    // Вычисляем палитру для этого тайла
                    int attrIndex = (tileY / 4) * 8 + (tileX / 4);
                    int rowInBlock = tileY % 4;
                    int colInBlock = tileX % 4;
                    uint8_t pal;
                    if (rowInBlock < 2)
                    {
                        if (colInBlock < 2)
                            pal = attributeTable[attrIndex].topLeft;
                        else
                            pal = attributeTable[attrIndex].topRight;
                    }
                    else
                    {
                        if (colInBlock < 2)
                            pal = attributeTable[attrIndex].bottomLeft;
                        else
                            pal = attributeTable[attrIndex].bottomRight;
                    }

                    // Устанавливаем текстуру (паттерн), позицию и палитру
                    background.setTexture(patternTable[tileIndex]);
                    background.setPosition(8.0f * tileX, 8.0f * tileY);
                    ColorRgb clr[4] = {nesToRgb(mPPU->mBackGroundPallete[pal].color1),
                                       nesToRgb(mPPU->mBackGroundPallete[pal].color2),
                                       nesToRgb(mPPU->mBackGroundPallete[pal].color3),
                                       nesToRgb(mPPU->mBackGroundPallete[pal].color4)};
                    background.setPallete(clr);
                    background.draw();
                }
            }
        }

        if (mCPU->memoryMap.mPPURegs->ppumask.show_sprites == 1)
        {
            if (mCPU->memoryMap.mPPURegs->ppuctrl.sprite_table == 0)
                patternTable = mPPU->mPatternTable0;
            else
                patternTable = mPPU->mPatternTable1;

            for (size_t i = 0; i < 64; i++)
            {
                ColorRgb clr[4] = {nesToRgb(mPPU->mSpritePallete[mCPU->oam[i].pallete].color1),
                                   nesToRgb(mPPU->mSpritePallete[mCPU->oam[i].pallete].color2),
                                   nesToRgb(mPPU->mSpritePallete[mCPU->oam[i].pallete].color3),
                                   nesToRgb(mPPU->mSpritePallete[mCPU->oam[i].pallete].color4)};
                sprite.setPallete(clr);
                sprite.setTexture(patternTable[mCPU->oam[i].tile8x8]);
                sprite.setPosition((float)mCPU->oam[i].x, (float)mCPU->oam[i].y);
                sprite.setFlips(mCPU->memoryMap.mMirror->oam[i].flipVertically, mCPU->memoryMap.mMirror->oam[i].flipHorizontally);
                sprite.draw();
            }
        }

        glfwSwapBuffers(mWindow);
    }
    mIsRunning->store(false);
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

Render::~Render()
{
    glfwTerminate();
}

ColorRgb Render::nesToRgb(uint8_t nesColor)
{
    return NES_PALETTE[nesColor];
}
