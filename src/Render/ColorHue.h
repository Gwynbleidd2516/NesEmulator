#pragma once

#include <stdint.h>

struct ColorHue
{
    uint8_t hue : 4;
    uint8_t value : 2;
};
