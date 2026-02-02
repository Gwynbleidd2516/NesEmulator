#include "NesProgram.h"
#include <fstream>

NesProgram::NesProgram()
{
}

void NesProgram::loadFile(string path)
{
#pragma pack(push, 1)
    struct Header
    {
        char format[4];
        unsigned char rpgSize;
        unsigned char chrSize;
        unsigned char trainer;
        unsigned char playChoice;
        unsigned char rpgRamSize;
        unsigned char tvSystem;
        unsigned char tvRpgRam;
        unsigned char padding[5];
    };
#pragma pack(pop)

    ifstream file;
    file.open(path, ios::in | ios::binary);
    Header header;
    file.read(reinterpret_cast<char *>(&header), sizeof(Header));

    if (strncmp(header.format, "NES", 3) != 0)
    {
        throw runtime_error("This is not a nes file");
    }

    mProcessor.loadFromFile(file, header.rpgSize / 2);
    mRender.loadFromFile(file, header.chrSize);

    file.close();
}

void NesProgram::step()
{
    mProcessor.doStep();
    mRender.show();
}

bool NesProgram::isEnd() const
{
    return mProcessor.eof() || !mRender.isOpen();
}