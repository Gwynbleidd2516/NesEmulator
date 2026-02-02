#ifndef STRUCTS
#define STRUCTS

#include <stack>
#include <vector>
#include "DiscreteVal.h"
using namespace std;

#pragma pack(push, 1)
struct CPU
{
    struct RAM
    {
        uint8_t zeroPage[416];
        uint8_t stack[96];
        struct OAM
        {
            uint8_t y;
            uint8_t tile;
            DiscreteVal attribute;
            uint8_t x;
        };
        OAM oam[64];
        uint8_t other[1280];
    };

    RAM mMirror[4];

    struct PPURegisters
    {
        DiscreteVal PPUCTRL;
        DiscreteVal PPUMASK;
        DiscreteVal PPUSTATUS;
        DiscreteVal OAMADDR;
        DiscreteVal OAMDATA;
        DiscreteVal PPUSCROLL;
        DiscreteVal PPUADDR;
        DiscreteVal PPUDATA;
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