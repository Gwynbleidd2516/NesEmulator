// #include "NesProgram.h"

// #include "Instructions.h"
// #include <iostream>
// using namespace std;

// uint16_t from8to16(uint8_t a1, uint8_t a2)
// {
//     uint16_t ans = a2;
//     ans *= 256;
//     return ans + a1;
// }

// void setReg(uint8_t &a1, const uint8_t &a2, Flags &flags)
// {
//     a1 = a2;

//     if (a1 == 0x00)
//         flags.Zero = true;
//     if ((int)(a1) < 0)
//         flags.Negative = true;
// }

// void setMem(uint8_t &a1, const uint8_t &a2, Flags &flags)
// {
//     a1 = a2;
// }

// // void setReg(uint8_t &a1, const uint8_t a2, Flags &flags)
// // {
// //     a1 = a2;
// //     if (a1 == 0x00)
// //         flags.Zero = true;
// //     if ((int)(a1) < 0)
// //         flags.Negative = true;
// // }

// map<uint8_t, function<void(FUNC_ARGS)>> NesProgram::mComands{
//     {uint8_t(LDA_IMMEDIATE), [](FUNC_ARGS)
//      {
//          //  reg.A = *(it + 1);
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          setReg(reg.A, *(it + 1), reg.flags);
//          it++;
//          cout << "LDA_IMMEDIATE\n";
//      }},

//     {uint8_t(LDA_ZERO_PAGE), [](FUNC_ARGS)
//      {
//          //  reg.A = ppu[(int)(*(it + 1))];
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          setReg(reg.A, ppu[(int)(*(it + 1))], reg.flags);
//          it++;
//          cout << "LDA_ZERO_PAGE\n";
//      }},

//     {uint8_t(LDA_ZERO_PAGE_X), [](FUNC_ARGS)
//      {
//          //  reg.A = ppu[(int)(*(it + 1 + reg.X))];
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          setReg(reg.A, ppu[(int)(*(it + 1 + reg.X))], reg.flags);
//          it++;
//          cout << "LDA_ZERO_PAGE_X\n";
//      }},

//     {uint8_t(LDA_ABSOLUTE), [](FUNC_ARGS)
//      {
//          //  int a = (int)from8to16(*(it + 1), *(it + 2));
//          //  reg.A = ppu[a];
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          setReg(reg.A, ppu[(int)from8to16(*(it + 1), *(it + 2))], reg.flags);
//          it += 2;
//          cout << "LDA_ABSOLUTE\n";
//      }},

//     {uint8_t(LDA_ABSOLUTE_X), [](FUNC_ARGS)
//      {
//          //  reg.A = ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.X];
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          setReg(reg.A, ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.X], reg.flags);
//          it += 2;
//          cout << "LDA_ABSOLUTE_X\n";
//      }},

//     {uint8_t(LDA_ABSOLUTE_Y), [](FUNC_ARGS)
//      {
//          //  reg.A = ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.Y];
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          setReg(reg.A, ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.Y], reg.flags);
//          it += 2;
//          cout << "LDA_ABSOLUTE_Y\n";
//      }},

//     {uint8_t(LDA_INDIRECT_X), [](FUNC_ARGS)
//      {
//          //  reg.A = ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.X];
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          uint16_t buff = ppu[(int)*(it + 1) + (int)reg.X];
//          setReg(reg.A, ppu[buff], reg.flags);
//          it += 1;
//          cout << "LDA_INDERECT_X\n";
//      }},

//     {uint8_t(LDA_INDIRECT_Y), [](FUNC_ARGS)
//      {
//          //  reg.A = ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.Y];
//          //  if (reg.A == 0x00)
//          //      reg.flags.Zero = true;
//          //  if ((int)(reg.A) < 0)
//          //      reg.flags.Negative = true;
//          uint16_t buff = ppu[(int)*(it + 1)];
//          setReg(reg.A, ppu[buff] + reg.Y, reg.flags);
//          it += 1;
//          cout << "LDA_INDERECT_Y\n";
//      }},

//     {uint8_t(STA_ZERO_PAGE), [](FUNC_ARGS)
//      {
//          setMem(ppu[(int)(*(it + 1))], reg.A, reg.flags);
//          it++;
//      }},

//     {uint8_t(STA_ZERO_PAGE_X), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)(*(it + 1 + reg.X))], reg.A, reg.flags);
//          it++;
//      }},

//     {uint8_t(STA_ABSOLUTE), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)from8to16(*(it + 1), *(it + 2))], reg.A, reg.flags);
//          it += 2;
//      }},

//     {uint8_t(STA_ABSOLUTE_X), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.X], reg.A, reg.flags);
//          it += 2;
//      }},

//     {uint8_t(STA_ABSOLUTE_Y), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.Y], reg.A, reg.flags);
//          it += 2;
//      }},

//     {uint8_t(STA_INDIRECT_X), [](FUNC_ARGS)
//      {
//          uint16_t buff = ppu[(int)*(it + 1) + (int)reg.X];
//          setReg(ppu[buff], reg.A, reg.flags);
//          it += 1;
//      }},

//     {uint8_t(STA_INDIRECT_Y), [](FUNC_ARGS)
//      {
//          uint16_t buff = ppu[(int)*(it + 1)] + (int)reg.Y;
//          setReg(ppu[buff], reg.A + reg.Y, reg.flags);
//          it += 1;
//      }},

//     {uint8_t(LDX_IMMEDIATE), [](FUNC_ARGS)
//      {
//          setReg(reg.X, *(it + 1), reg.flags);
//          it++;
//      }},

//     {uint8_t(LDX_ZERO_PAGE), [](FUNC_ARGS)
//      {
//          setReg(reg.X, ppu[(int)(*(it + 1))], reg.flags);
//          it++;
//      }},

//     {uint8_t(LDX_ZERO_PAGE_Y), [](FUNC_ARGS)
//      {
//          setReg(reg.X, ppu[(int)(*(it + 1) + (int)reg.Y)], reg.flags);
//          it++;
//      }},

//     {uint8_t(LDX_ABSOLUTE), [](FUNC_ARGS)
//      {
//          setReg(reg.X, ppu[(int)from8to16(*(it + 1), *(it + 2))], reg.flags);
//          it += 2;
//      }},

//     {uint8_t(LDX_ABSOLUTE_Y), [](FUNC_ARGS)
//      {
//          setReg(reg.X, ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.Y], reg.flags);
//          it += 2;
//      }},

//     {uint8_t(STX_ZERO_PAGE), [](FUNC_ARGS)
//      {
//          setMem(ppu[(int)(*(it + 1))], reg.X, reg.flags);
//          it++;
//      }},

//     {uint8_t(STX_ZERO_PAGE_Y), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)(*(it + 1 + reg.Y))], reg.X, reg.flags);
//          it++;
//      }},

//     {uint8_t(STX_ABSOLUTE), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)from8to16(*(it + 1), *(it + 2))], reg.X, reg.flags);
//          it += 2;
//      }},

//     {uint8_t(LDY_IMMEDIATE), [](FUNC_ARGS)
//      {
//          setReg(reg.Y, *(it + 1), reg.flags);
//          it++;
//      }},

//     {uint8_t(LDY_ZERO_PAGE), [](FUNC_ARGS)
//      {
//          setReg(reg.Y, ppu[(int)(*(it + 1))], reg.flags);
//          it++;
//      }},

//     {uint8_t(LDY_ZERO_PAGE_X), [](FUNC_ARGS)
//      {
//          setReg(reg.Y, ppu[(int)(*(it + 1) + (int)reg.X)], reg.flags);
//          it++;
//      }},

//     {uint8_t(LDY_ABSOLUTE), [](FUNC_ARGS)
//      {
//          setReg(reg.Y, ppu[(int)from8to16(*(it + 1), *(it + 2))], reg.flags);
//          it += 2;
//      }},

//     {uint8_t(LDY_ABSOLUTE_X), [](FUNC_ARGS)
//      {
//          setReg(reg.Y, ppu[(int)from8to16(*(it + 1), *(it + 2)) + (int)reg.X], reg.flags);
//          it += 2;
//      }},

//     {uint8_t(STY_ZERO_PAGE), [](FUNC_ARGS)
//      {
//          setMem(ppu[(int)(*(it + 1))], reg.Y, reg.flags);
//          it++;
//      }},

//     {uint8_t(STY_ZERO_PAGE_X), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)(*(it + 1 + reg.X))], reg.Y, reg.flags);
//          it++;
//      }},

//     {uint8_t(STY_ABSOLUTE), [](FUNC_ARGS)
//      {
//          setReg(ppu[(int)from8to16(*(it + 1), *(it + 2))], reg.Y, reg.flags);
//          it += 2;
//      }},

//     {uint8_t(TAX), [](FUNC_ARGS)
//      {
//          setReg(reg.X, reg.A, reg.flags);
//          it++;
//      }},

//     {uint8_t(TXA), [](FUNC_ARGS)
//      {
//          setReg(reg.A, reg.X, reg.flags);
//          it++;
//      }},

//     {uint8_t(TAY), [](FUNC_ARGS)
//      {
//          setReg(reg.Y, reg.A, reg.flags);
//          it++;
//      }},

//     {uint8_t(TYA), [](FUNC_ARGS)
//      {
//          setReg(reg.A, reg.Y, reg.flags);
//          it++;
//      }}

// };