#ifndef PPU_
#define PPU_

#include "Pattern.h"
#include "AttributeTable.h"
#include "Logs.h"

#pragma pack(push, 1)
struct PPU
{
    Pattern mPatternTable0[256];
    Pattern mPatternTable1[256];

    uint8_t mNametable0[30][32];
    AttributeTable mAttributeTable0[0x40];
    uint8_t mNametable1[30][32];
    AttributeTable mAttributeTable1[0x40];
    uint8_t mNametable2[30][32];
    AttributeTable mAttributeTable2[0x40];
    uint8_t mNametable3[30][32];
    AttributeTable mAttributeTable3[0x40];

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

    uint8_t read(size_t i)
    {
#ifdef DO_LOGS
        Logs::GetInstance().ppu->info("Read from ${:x} - {:x}", i, at(i));
#endif
        return at(i);
    }

    void write(size_t i, uint8_t val)
    {
        uint8_t buf = at(i);
        if (i >= 0x2000 && i < 0x23C0)
        {
            at(i + 0x400) = val;
        }
        else if (i >= 0x2400 && i < 0x27C0)
        {
            at(i - 0x400) = val;
        }
        else if (i >= 0x2800 && i < 0x2BC0)
        {
            at(i + 0x400) = val;
        }
        else if (i >= 0x2C00 && i < 0x2FC0)
        {
            at(i - 0x400) = val;
        }

        at(i) = val;
#ifdef DO_LOGS
        Logs::GetInstance().ppu->info("Written in ${:x}; {:x} -> {:x}", i, buf, val);
#endif
    }
};
#pragma pack(pop)

#endif