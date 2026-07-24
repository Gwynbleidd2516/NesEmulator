#ifndef STRUCTS
#define STRUCTS

#include <stack>
#include <vector>
using namespace std;

#include "Ppu.h"
#include "Struct16_t.h"

struct OAM
{
    uint8_t y;
    uint8_t bank : 1;
    uint8_t tile : 7;
    uint8_t pallete : 2;
    uint8_t unimplemented : 3;
    bool flipHorizontally : 1;
    bool flipVertically : 1;
    uint8_t x;
};

struct PPUCTRL
{
    union
    {
        uint8_t raw;
        struct
        {
            uint8_t nametable : 2;        // биты 0-1: базовый неймтейбл (0-3)
            uint8_t increment : 1;        // бит 2:   инкремент адреса VRAM (0: +1, 1: +32)
            uint8_t sprite_table : 1;     // бит 3:   таблица паттернов для спрайтов (0: $0000, 1: $1000)
            uint8_t background_table : 1; // бит 4:   таблица паттернов для фона (0: $0000, 1: $1000)
            uint8_t sprite_size : 1;      // бит 5:   размер спрайтов (0: 8x8, 1: 8x16)
            uint8_t master_slave : 1;     // бит 6:   режим PPU (обычно 0)
            uint8_t nmi_enable : 1;       // бит 7:   разрешение NMI при vblank
        };
    };
};

struct PPUMASK
{
    union
    {
        uint8_t raw;
        struct
        {
            uint8_t grayscale : 1;          // бит 0: ч/б режим
            uint8_t show_background_l8 : 1; // бит 1: показывать фон в левых 8 пикселях
            uint8_t show_sprites_l8 : 1;    // бит 2: показывать спрайты в левых 8 пикселях
            uint8_t show_background : 1;    // бит 3: включить фон
            uint8_t show_sprites : 1;       // бит 4: включить спрайты
            uint8_t emphasize_red : 1;      // бит 5: усиление красного
            uint8_t emphasize_green : 1;    // бит 6: усиление зелёного
            uint8_t emphasize_blue : 1;     // бит 7: усиление синего
        };
    };
};

struct PPUSTATUS
{
    union
    {
        uint8_t raw;
        struct
        {
            uint8_t unused : 5;          // биты 0-4: не используются (open bus)
            uint8_t sprite_overflow : 1; // бит 5:   переполнение спрайтов
            uint8_t sprite0_hit : 1;     // бит 6:   столкновение спрайта 0 с фоном
            uint8_t vblank : 1;          // бит 7:   начало вертикального гашения
        };
    };
};

using OAMADDR = uint8_t;

using OAMDATA = uint8_t;

using PPUSCROLL = uint8_t;

using PPUADDR = uint8_t;

using PPUDATA = uint8_t;

struct PPURegisters
{
    PPUCTRL ppuctrl;
    PPUMASK ppumask;
    PPUSTATUS ppustatus;
    OAMADDR oamaddr;
    OAMDATA oamdata;
    PPUSCROLL ppuscroll;
    PPUADDR ppuaddr;
    PPUDATA ppudata;
};

#pragma pack(push, 1)
struct MemoryMap
{
    struct RAM
    {
        uint8_t zeroPage[416];
        uint8_t stack[96];
        OAM oam[64];
        uint8_t other[1280];
    };

    RAM mMirror[4];
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
    // uint8_t rom[32768];
    struct
    {
        uint8_t rom1[0x4000];
        uint8_t rom2[0x4000 - 0x6];
        Struct16_t nmiVector;
        Struct16_t resetVector;
        Struct16_t irqVector;
    };

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

struct CPU
{
    uint8_t ppuscrolly;
    bool ppuscrollLatch = false;

    uint8_t ppuaddr_write : 6;
    bool ppuaddrLatch = false;

    PPU *ppu;

    MemoryMap memoryMap;
    uint8_t &operator[](size_t i)
    {
        return memoryMap[i];
    }

    uint8_t &at(size_t i)
    {
        return memoryMap.at(i);
    }

    void write(size_t i, uint8_t value)
    {
        uint8_t buf = memoryMap[i];
        switch (i)
        {
        case 0x2004:
            memoryMap[0x200 + memoryMap.mPPURegs->oamaddr++] = value;
            break;

        case 0x2005:
            if (ppuscrollLatch)
                memoryMap[i] = value;
            else
                ppuscrolly = value;
            ppuscrollLatch = !ppuscrollLatch;
            break;

        case 0x2006:
            if (ppuaddrLatch)
                memoryMap[i] = value;
            else
                ppuaddr_write = value;
            ppuaddrLatch = !ppuaddrLatch;
            break;

        case 0x2007:
            ppu->write(memoryMap.mPPURegs->ppuaddr, value);
            memoryMap.mPPURegs->ppuaddr += 1 + memoryMap.mPPURegs->ppuctrl.increment * 31;
            break;
        case 0x4014:
            memoryMap[0x200 + memoryMap.mPPURegs->oamaddr++] = value;
            break;
        default:
            at(i) = value;
            break;
        }
#ifdef DO_LOGS
        Logs::GetInstance().ram->info("Written in ${:x}; {:x} -> {:x}", i, buf, value);
#endif
    }

    uint8_t read(size_t i)
    {
        uint8_t buf = memoryMap[i];
        uint8_t ans;
        switch (i)
        {
        case 0x2002:
            // memoryMap.mPPURegs->ppustatus.vblank = false;
            ppuaddrLatch = false;
            ppuscrollLatch = false;
            return memoryMap.mPPURegs->ppustatus.raw;
            break;

        case 0x2004:
            return memoryMap[0x200 + memoryMap.mPPURegs->oamaddr++];
            break;

        case 0x2007:
            ans = ppu->read(memoryMap.mPPURegs->ppuaddr);
            memoryMap.mPPURegs->ppuaddr += 1 + memoryMap.mPPURegs->ppuctrl.increment * 31;
            return ans;
            break;

        default:
            return at(i);
            break;
        }
#ifdef DO_LOGS
        Logs::GetInstance().ram->info("Read from ${:x} - {}", i, buf);
#endif
    }
};

#endif