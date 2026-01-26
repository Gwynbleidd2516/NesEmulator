#ifndef STRUCTS
#define STRUCTS

#include <stack>
#include <vector>
using namespace std;

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

#pragma pack(push, 1)
struct NesFile
{
    Header head;
    vector<uint8_t> code;
    vector<uint8_t> data;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CPU
{
    struct RAM
    {
        uint8_t zeroPage[256];
        uint8_t stack[256];
        uint8_t OAM[1536];
    };

    RAM mMirror[4];

    struct PPURegisters
    {
        uint8_t PPUCTRL;
        uint8_t PPUMASK;
        uint8_t PPUSTATUS;
        uint8_t OAMADDR;
        uint8_t OAMDATA;
        uint8_t PPUSCROLL;
        uint8_t PPUADDR;
        uint8_t PPUDATA;
    };

    PPURegisters mPPURegs[1024];

    struct APU
    {
        uint32_t pulse1;
        uint32_t pulse2;
        uint32_t triangle;
        uint32_t noise;
        uint32_t dmc;
        uint16_t status;
        uint16_t frameCounter;
    };

    APU apu;

    struct TestMode
    {
        uint16_t A03G;
        uint16_t A03E;
        uint16_t A07A;
        uint16_t A03;
    };

    TestMode testMode;

    uint8_t expansionROM[8160];

    uint8_t ram[8192];
    uint8_t rom[32768];

    uint8_t &operator[](size_t i)
    {
        return *((uint8_t *)this + i);
    }

    uint8_t &at(size_t i)
    {
        return *((uint8_t *)this + i);
    }
};

#pragma pack(pop)

#endif