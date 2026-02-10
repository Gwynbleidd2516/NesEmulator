#ifndef PPU_
#define PPU_

#include <stdint.h>

#pragma pack(push, 1)
struct PPU
{
    uint8_t mPatternTable0[0x1000];
    uint8_t mPatternTable1[0x1000];

    uint8_t mNametable0[0x3c0];
    uint8_t mAttributeTable0[0x40];
    uint8_t mNametable1[0x3c0];
    uint8_t mAttributeTable1[0x40];
    uint8_t mNametable2[0x3c0];
    uint8_t mAttributeTable2[0x40];
    uint8_t mNametable3[0x3c0];
    uint8_t mAttributeTable3[0x40];

    uint8_t mUnused[0xF00];
    uint8_t mPallete[0x20];
    uint8_t mPalleteMirror[0xE0];

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