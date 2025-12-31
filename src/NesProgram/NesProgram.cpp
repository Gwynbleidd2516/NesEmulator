#include "NesProgram.h"
#include <fstream>

NesProgram::NesProgram()
{
    mPPU.resize(0x10000);
    mInstructions =
        {
            //          0        1        2
            /* 0 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 1 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 2 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 3 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 4 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 5 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 6 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 7 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 8 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* 9 */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* A */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* B */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* C */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* D */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* E */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* F */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}};
}

void NesProgram::loadFile(string path)
{
    ifstream file;
    file.open(path, ios::in | ios::binary);
    file.read(reinterpret_cast<char *>(&mNesFile.head), sizeof(Header));

    mNesFile.code = vector<uint8_t>(16384 * mNesFile.head.rpgSize);
    for (auto &p : mNesFile.code)
    {
        file.read((char *)&p, sizeof(uint8_t));
    }

    mNesFile.data = vector<uint8_t>(8192 * mNesFile.head.chrSize);
    for (auto &p : mNesFile.data)
    {
        file.read((char *)&p, sizeof(uint8_t));
    }

    file.close();
    mCodeIt = mNesFile.code.begin();
}

void NesProgram::step()
{
    // if (mComands.count(*mCodeIt) != 0)
    //     mComands[*mCodeIt](mCodeIt, mRegisters, mPPU);
    mCodeIt++;
}

bool NesProgram::eof() const
{
    return mCodeIt == mNesFile.code.end();
}