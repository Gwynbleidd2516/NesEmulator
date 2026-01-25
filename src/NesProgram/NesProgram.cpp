#include "NesProgram.h"
#include <fstream>

#include "Load.h"
#include "Store.h"
#include "Add.h"
#include "Dec.h"
#include "Inc.h"
#include "Sub.h"
#include "And.h"
#include "Bit.h"
#include "Eor.h"
#include "Ora.h"
#include "Bcc.h"
#include "Bcs.h"
#include "Beq.h"
#include "Bmi.h"
#include "Bne.h"
#include "Bpl.h"
#include "Bvc.h"
#include "Bvs.h"
#include "Cmp.h"
#include "SetFlag.h"
#include "Brk.h"
#include "Jmp.h"
#include "Jsr.h"
#include "Rti.h"
#include "Rts.h"
#include "Nop.h"
#include "Asl.h"
#include "Lsr.h"
#include "Rol.h"
#include "Ror.h"
#include "PullAcc.h"
#include "PullProcStatus.h"
#include "PushAcc.h"
#include "PushProcStatus.h"
#include "TransferFromStack.h"
#include "TransferToStack.h"
#include "Transfer.h"

#include "Indirect.h"
#include"IndirectInd.h"
#include "ZeroPage.h"
#include "Immediate.h"
#include "Implied.h"
#include "Absolute.h"

NesProgram::NesProgram()
{
    mPPU.resize(0x10000);
    mInstructions =
        {
            //          0        1        2
            /* 00 */ {make_shared<Brk>(mNesFile.code.begin(), nullptr, &mRegisters.sp, &mRegisters.flags),
                      make_shared<Ora>(&mRegisters.A, new IndirectX(mPPU, &mRegisters.X), &mRegisters.flags),
                      nullptr, nullptr, nullptr,
                      make_shared<Ora>(&mRegisters.A, new ZeroPage(mPPU), &mRegisters.flags),
                      make_shared<Asl>(new ZeroPage(mPPU), &mRegisters.flags),
                      nullptr,
                      make_shared<PushProcStatus>(&mRegisters.flags, &mRegisters.sp),
                      make_shared<Ora>(&mRegisters.A, new Immediate(), &mRegisters.flags),
                      make_shared<Asl>(new Implied(&mRegisters.A), &mRegisters.flags),
                      nullptr, nullptr,
                      make_shared<Ora>(&mRegisters.A, new Absolute(mPPU), &mRegisters.flags),
                      make_shared<Asl>(new Absolute(mPPU), &mRegisters.flags),
                      nullptr},
            /* 1 */ {make_shared<Bpl>(&mCodeIt, new Immediate(), &mRegisters.flags),
                     make_shared<Ora>(&mRegisters.A, new IndirectY(mPPU, &mRegisters.Y), &mRegisters.flags),
                    nullptr, nullptr, nullptr, 
                     make_shared<Ora>(&mRegisters.A, new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Asl>(new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                    nullptr, 
                     make_shared<SetFlag>(&mRegisters.flags.Carry, false), 
                     make_shared<Ora>(&mRegisters.A, new AbsoluteInd(mPPU, &mRegisters.Y), &mRegisters.flags),
                    nullptr, nullptr, nullptr, 
                     make_shared<Ora>(&mRegisters.A, new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Asl>(new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                    nullptr},
            /* 2 */ {make_shared<Jsr>(mPPU.begin(), &mCodeIt, &mRegisters.sp), 
                     make_shared<And>(new IndirectX(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr, 
                     make_shared<Bit>(&mRegisters.A, new ZeroPage(mPPU), &mRegisters.flags),
                     make_shared<And>(new ZeroPage(mPPU), &mRegisters.flags),
                     make_shared<Rol>(new ZeroPage(mPPU), &mRegisters.flags),
                     nullptr, 
                     make_shared<PullProcStatus>(&mRegisters.flags, &mRegisters.sp), 
                     make_shared<And>(new Immediate(), &mRegisters.flags),
                     make_shared<Rol>(new Implied(&mRegisters.A), &mRegisters.flags),
                     nullptr, 
                     make_shared<Bit>(&mRegisters.A, new Absolute(mPPU), &mRegisters.flags),
                     make_shared<And>(new Absolute(mPPU), &mRegisters.flags),
                     make_shared<Rol>(new Absolute(mPPU), &mRegisters.flags),
                     nullptr},
            /* 3 */ {make_shared<Bmi>(&mCodeIt, new Immediate(), &mRegisters.flags), 
                     make_shared<And>(new IndirectY(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<And>(new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Rol>(new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, 
                     make_shared<SetFlag>(&mRegisters.flags.Carry, true), 
                     make_shared<And>(new AbsoluteInd(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<And>(new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Rol>(new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr},
            /* 4 */ {make_shared<Rti>(mPPU.begin(), &mRegisters.sp, &mRegisters.flags), 
                     make_shared<Eor>(&mRegisters.A, new IndirectX(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<Eor>(&mRegisters.A, new ZeroPage(mPPU), &mRegisters.flags),
                     make_shared<Lsr>(new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, 
                     make_shared<PushAcc>(&mRegisters.A, &mRegisters.sp), 
                     make_shared<Eor>(&mRegisters.A, new Immediate(), &mRegisters.flags),
                     make_shared<Lsr>(new Implied(&mRegisters.A), &mRegisters.flags),
                     nullptr, 
                     make_shared<Jmp>(new Absolute(mPPU), &mRegisters.flags),
                     make_shared<Eor>(&mRegisters.A, new Absolute(mPPU), &mRegisters.flags),
                     make_shared<Lsr>(new Absolute(mPPU), &mRegisters.flags),
                     nullptr},
            /* 5 */ {make_shared<Bvc>(&mCodeIt, new Immediate, &mRegisters.flags), 
                     make_shared<Eor>(&mRegisters.A, new IndirectY(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<Eor>(&mRegisters.A, new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Lsr>(new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, 
                     make_shared<SetFlag>(&mRegisters.flags.Interrupt, false), 
                     make_shared<Eor>(&mRegisters.A, new AbsoluteInd(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<Eor>(&mRegisters.A, new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Lsr>(new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr},
            /* 6 */ {make_shared<Rts>(mPPU.begin(), nullptr, &mRegisters.sp), 
                     make_shared<Add>(&mRegisters.A, new IndirectX(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<Add>(&mRegisters.A, new ZeroPage(mPPU), &mRegisters.flags),
                     make_shared<Ror>(new ZeroPage(mPPU), &mRegisters.flags),
                     nullptr, 
                     make_shared<PullAcc>(&mRegisters.A, &mRegisters.sp, &mRegisters.flags), 
                     make_shared<Add>(&mRegisters.A, new Immediate, &mRegisters.flags),
                     make_shared<Ror>(new Implied(&mRegisters.A), &mRegisters.flags),
                     nullptr, 
                     make_shared<Jmp>(new Indirect(mPPU), &mRegisters.flags),
                     make_shared<Add>(&mRegisters.A, new Absolute(mPPU), &mRegisters.flags),
                     make_shared<Ror>(new Absolute(mPPU), &mRegisters.flags),
                     nullptr},
            /* 7 */ {make_shared<Bvs>(&mCodeIt, new Immediate, &mRegisters.flags), 
                     make_shared<Add>(&mRegisters.A, new IndirectY(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<Add>(&mRegisters.A, new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Ror>(new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, 
                     make_shared<SetFlag>(&mRegisters.flags.Interrupt, true), 
                     make_shared<Add>(&mRegisters.A, new AbsoluteInd(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, nullptr, nullptr, 
                     make_shared<Add>(&mRegisters.A, new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Ror>(new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr},
            /* 8 */ {nullptr, 
                     make_shared<Store>(new Implied(&mRegisters.A), new IndirectX(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr, 
                     make_shared<Store>(new Implied(&mRegisters.Y), new ZeroPage(mPPU), &mRegisters.flags),
                     make_shared<Store>(new Implied(&mRegisters.A), new ZeroPage(mPPU), &mRegisters.flags),
                     make_shared<Store>(new Implied(&mRegisters.X), new ZeroPage(mPPU), &mRegisters.flags),
                     nullptr, 
                     make_shared<Dec>(new Implied(&mRegisters.Y), &mRegisters.flags),
                     nullptr, 
                     make_shared<Transfer>(new Implied(&mRegisters.A), new Implied(&mRegisters.X), &mRegisters.flags),
                     nullptr, 
                     make_shared<Store>(new Implied(&mRegisters.Y), new Absolute(mPPU), &mRegisters.flags),
                     make_shared<Store>(new Implied(&mRegisters.A), new Absolute(mPPU), &mRegisters.flags),
                     make_shared<Store>(new Implied(&mRegisters.X), new Absolute(mPPU), &mRegisters.flags),
                     nullptr},
            /* 9 */ {make_shared<Bcc>(&mCodeIt, new Immediate, &mRegisters.flags), 
                     make_shared<Store>(new Implied(&mRegisters.A), new IndirectY(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, nullptr, 
                     make_shared<Store>(new Implied(&mRegisters.Y), new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Store>(new Implied(&mRegisters.A), new ZeroPageInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     make_shared<Store>(new Implied(&mRegisters.X), new ZeroPageInd(mPPU, &mRegisters.Y), &mRegisters.flags),
                     nullptr, 
                     make_shared<Transfer>(new Implied(&mRegisters.A), new Implied(&mRegisters.Y), &mRegisters.flags),
                     make_shared<Store>(new Implied(&mRegisters.A), new AbsoluteInd(mPPU, &mRegisters.Y), &mRegisters.flags),
                     make_shared<TransferToStack>(&mRegisters.X, &mRegisters.sp),
                     nullptr, nullptr, 
                     make_shared<Store>(new Implied(&mRegisters.A), new AbsoluteInd(mPPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr},
            /* A */ {make_shared<Load>(new Implied(&mRegisters.Y), new Immediate, &mRegisters.flags), 
                     make_shared<Load>(new Implied(&mRegisters.A), new IndirectX(mPPU, &mRegisters.X), &mRegisters.flags), 
                     make_shared<Load>(new Implied(&mRegisters.X), new Immediate, &mRegisters.flags), 
                     nullptr, 
                     make_shared<Load>(new Implied(&mRegisters.Y), new ZeroPage(mPPU), &mRegisters.flags), 
                     make_shared<Load>(new Implied(&mRegisters.A), new ZeroPage(mPPU), &mRegisters.flags), 
                     make_shared<Load>(new Implied(&mRegisters.X), new ZeroPage(mPPU), &mRegisters.flags), 
                     nullptr, 
                     make_shared<Transfer>(new Implied(&mRegisters.Y), new Implied(&mRegisters.A), &mRegisters.flags),
                     make_shared<Load>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), 
                     make_shared<Transfer>(new Implied(&mRegisters.X), new Implied(&mRegisters.A), &mRegisters.flags),
                     nullptr, 
                     make_shared<Load>(new Implied(&mRegisters.Y), new Absolute(mPPU), &mRegisters.flags), 
                     make_shared<Load>(new Implied(&mRegisters.A), new Absolute(mPPU), &mRegisters.flags), 
                     make_shared<Load>(new Implied(&mRegisters.X), new Absolute(mPPU), &mRegisters.flags), 
                     nullptr},
            /* B */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* C */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* D */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* E */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            /* F */ {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}};
}

void NesProgram::loadFile(string path)
{
    ifstream file;
    file.open(path, ios::in | ios::binary);
    file.read(reinterpret_cast<char *>(&mNesFile.head), sizeof(Header));

    mNesFile.code = vector<uint8_t>(16384 * mNesFile.head.rpgSize);
    for (auto &p : mNesFile.code)
    {
        file.read((char *)&p, sizeof(uint8_t));
    }

    mNesFile.data = vector<uint8_t>(8192 * mNesFile.head.chrSize);
    for (auto &p : mNesFile.data)
    {
        file.read((char *)&p, sizeof(uint8_t));
    }

    file.close();
    mCodeIt = mNesFile.code.begin();
}

void NesProgram::step()
{
    // if (mComands.count(*mCodeIt) != 0)
    //     mComands[*mCodeIt](mCodeIt, mRegisters, mPPU);
    mCodeIt++;
}

bool NesProgram::eof() const
{
    return mCodeIt == mNesFile.code.end();
}