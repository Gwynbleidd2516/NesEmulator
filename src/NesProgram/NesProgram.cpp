#include "NesProgram.h"
#include <fstream>

NesProgram::NesProgram()
{
}

void NesProgram::loadFile(string path)
{
    ifstream file;
    file.open(path, ios::in | ios::binary);
    file.read(reinterpret_cast<char *>(&mHeader), sizeof(Header));

    if (strncmp(mHeader.format, "NES", 3) != 0)
    {
        throw runtime_error("This is not a nes file");
    }

    mProcessor.setHeader(mHeader);

    mProcessor.loadFromFile(file);
    mRender.loadFromFile(file);

    file.close();

    mProcessor.getCPU()->ppu = mRender.getPPU();
    mRender.setOAM(mProcessor.getOAM());
}

void NesProgram::step()
{
    mProcessor.doStep();
    // mRender.show();
}

bool NesProgram::isEnd() const
{
    return mProcessor.eof(); //|| !mRender.isOpen();
}

void NesProgram::reset()
{
    mProcessor.reset();
}
