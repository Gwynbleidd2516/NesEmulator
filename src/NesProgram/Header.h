#pragma once
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