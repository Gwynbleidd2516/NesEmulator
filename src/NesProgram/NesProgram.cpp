#include "NesProgram.h"
#include <fstream>

NesProgram::NesProgram()
{
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

    mProcessor.setHeader(mHeader);

    if (mHeader.rpgSize == 1)
    {
        stream.read(reinterpret_cast<char *>(&mCPU.memoryMap.rom2), 0x4000 * mHeader.rpgSize);
    }
    else
    {
        stream.read(reinterpret_cast<char *>(&mCPU.memoryMap.rom1), 0x4000 * mHeader.rpgSize);
    }
    mCPU.memoryMap.mPPURegs->ppuctrl.nmi_enable = false;

    stream.read(reinterpret_cast<char *>(&mPPU), 0x2000 * mHeader.chrSize);
    mCPU.ppu = &mPPU;
    mProcessor.setCPU(&mCPU);
    mRender.setCPU(&mCPU);
    mRender.setPPU(&mPPU);
}

void NesProgram::step()
{
    mProcessor.launch();
    mRender.show();
}

bool NesProgram::isEnd() const
{
    return !mRender.isOpen();
}

void NesProgram::reset()
{
    mProcessor.reset();
}
