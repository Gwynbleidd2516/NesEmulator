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

#endif