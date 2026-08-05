#include "NesProgram.h"
#include <fstream>
#include <iostream>

NesProgram::NesProgram() : mProcessorThread(&NesProgram::processThread, this),
                           mRenderThread(&NesProgram::renderThread, this)
{
    memset(&mCPU.memoryMap, 0x0, sizeof(MemoryMap));
    memset(&mPPU, 0x0, sizeof(PPU));
}

void NesProgram::loadFromFile(string path)
{
    ifstream file;
    file.open(path, ios::in | ios::binary);
    loadFromStream(file);
    file.close();
}

void NesProgram::loadFromStream(istream &stream)
{
    stream.read(reinterpret_cast<char *>(&mHeader), sizeof(Header));

    if (strncmp(mHeader.format, "NES", 3) != 0)
    {
        throw runtime_error("This is not a nes file");
    }

    if (mHeader.rpgSize == 1)
    {
        stream.read(reinterpret_cast<char *>(&mCPU.memoryMap.rom2), 0x4000 * mHeader.rpgSize);
    }
    else
    {
        stream.read(reinterpret_cast<char *>(&mCPU.memoryMap.rom1), 0x4000 * mHeader.rpgSize);
    }

    stream.read(reinterpret_cast<char *>(&mPPU), 0x2000 * mHeader.chrSize);
    mCPU.ppu = &mPPU;
    mIsRunningAtomic.store(true);
    mLoadSemaphoreProc.release();
    mLoadSemaphoreRender.release();
}

void NesProgram::step()
{
    mLaunchSemaphoreProc.release();
    mLaunchSemaphoreRender.release();
    mProcessorThread.join();
    mRenderThread.join();
}

void NesProgram::reset()
{
    // mProcessor.reset();
}

void NesProgram::processThread()
{
    mLoadSemaphoreProc.acquire();

    Processor processor;
    processor.setHeader(mHeader);
    processor.setCPU(&mCPU);
    processor.setIsRunningAtomic(&mIsRunningAtomic);
    mLaunchSemaphoreProc.acquire();

    processor.reset();
    processor.launch();
}

void NesProgram::renderThread()
{
    mLoadSemaphoreRender.acquire();

    Render render;
    render.setCPU(&mCPU);
    render.setPPU(&mPPU);
    render.setIsRunningAtomic(&mIsRunningAtomic);
    mLaunchSemaphoreRender.acquire();

    render.show();
}
