#pragma once

#include <stdint.h>

struct ColorRgb
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ColorRgbFloat
{
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;

    ColorRgbFloat &operator=(const ColorRgb &color)
    {
        r = (float)color.r / 255.0f;
        g = (float)color.g / 255.0f;
        b = (float)color.b / 255.0f;
        return *this;
    }
};