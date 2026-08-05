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
    // while (mWindow.isOpen())
    // {
    //     // mWindow.clear();
    //     mView.setCenter({mCPU->ppuscrollx + 256.f / 2.f, mCPU->memoryMap.mPPURegs->ppuscroll + 240.0f / 2.f});
    //     mWindow.setView(mView);
    //     mWindow.display();
    // }
    NesSprite sprite;
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Pattern *patternTable;

        if (mCPU->memoryMap.mPPURegs->ppumask.show_background == 1)
        {
            if (mCPU->memoryMap.mPPURegs->ppuctrl.background_table == 0)
                patternTable = mPPU->mPatternTable0;
            else
                patternTable = mPPU->mPatternTable1;

            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    sprite.setTexture(patternTable[mPPU->mNametable0[i][j]]);
                    sprite.setPosition(8.0f * j, 8.0f * i);
                    sprite.draw();

                    sprite.setTexture(patternTable[mPPU->mNametable1[i][j]]);
                    sprite.setPosition(8.0f * j + 256.0f, 8.0f * i);
                    sprite.draw();

                    sprite.setTexture(patternTable[mPPU->mNametable2[i][j]]);
                    sprite.setPosition(8.0f * j, 8.0f * i + 240.0f);
                    sprite.draw();

                    sprite.setTexture(patternTable[mPPU->mNametable3[i][j]]);
                    sprite.setPosition(8.0f * j + 256.0f, 8.0f * i + 240.0f);
                    sprite.draw();
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
                sprite.setTexture(patternTable[mCPU->oam[i].tile8x8]);
                sprite.setPosition((float)mCPU->oam[i].x, (float)mCPU->oam[i].y);
                // buf.setScale(Vector2f(-1.0f + !mCPU->memoryMap.mMirror->oam[i].flipHorizontally * 2.0f,
                //                       -1.0f + !mCPU->memoryMap.mMirror->oam[i].flipVertically * 2.0f));
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
