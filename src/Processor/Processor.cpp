#include "Processor.h"

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
#include "IndirectInd.h"
#include "ZeroPage.h"
#include "Immediate.h"
#include "Implied.h"
#include "Absolute.h"
#include "Empty.h"

Processor::Processor()
{
    mRegisters.pc = &mCPU.rom[0];
    mRegisters.sp = &mCPU.mMirror[0].stack[0];
    mCPU.mPPURegs[0].PPUCTRL.setBit(0, true);
    mCPU.mPPURegs[0].PPUSTATUS.setBit(6, true);
    mInstructions =
        {
            //          0        1        2
            /* 0 */ {make_shared<Brk>(&mCPU, new Empty, &mRegisters.sp, &mRegisters.flags),
                     make_shared<Ora>(&mRegisters.A, new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr, nullptr,
                     make_shared<Ora>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags),
                     make_shared<Asl>(new ZeroPage(mCPU), &mRegisters.flags),
                     nullptr,
                     make_shared<PushProcStatus>(&mRegisters.flags, &mRegisters.sp),
                     make_shared<Ora>(&mRegisters.A, new Immediate(), &mRegisters.flags),
                     make_shared<Asl>(new Implied(&mRegisters.A), &mRegisters.flags),
                     nullptr, nullptr,
                     make_shared<Ora>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags),
                     make_shared<Asl>(new Absolute(mCPU), &mRegisters.flags),
                     nullptr},
            /* 1 */ {make_shared<Bpl>(&mRegisters.pc, new Immediate(), &mRegisters.flags), make_shared<Ora>(&mRegisters.A, new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Ora>(&mRegisters.A, new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Asl>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetFlag>(&mRegisters.flags.Carry, false), make_shared<Ora>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Ora>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Asl>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 2 */ {make_shared<Jsr>(&mCPU, new Absolute(mCPU), &mRegisters.sp), make_shared<And>(&mRegisters.A, new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Bit>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags), make_shared<And>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags), make_shared<Rol>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_shared<PullProcStatus>(&mRegisters.flags, &mRegisters.sp), make_shared<And>(&mRegisters.A, new Immediate(), &mRegisters.flags), make_shared<Rol>(new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Bit>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags), make_shared<And>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags), make_shared<Rol>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 3 */ {make_shared<Bmi>(&mRegisters.pc, new Immediate(), &mRegisters.flags), make_shared<And>(&mRegisters.A, new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<And>(&mRegisters.A, new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Rol>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetFlag>(&mRegisters.flags.Carry, true), make_shared<And>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<And>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Rol>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 4 */ {make_shared<Rti>(&mCPU, &mRegisters.sp, &mRegisters.flags), make_shared<Eor>(&mRegisters.A, new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Eor>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags), make_shared<Lsr>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<PushAcc>(&mRegisters.A, &mRegisters.sp), make_shared<Eor>(&mRegisters.A, new Immediate(), &mRegisters.flags), make_shared<Lsr>(new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Jmp>(&mCPU, new Absolute(mCPU), &mRegisters.sp), make_shared<Eor>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags), make_shared<Lsr>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 5 */ {make_shared<Bvc>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_shared<Eor>(&mRegisters.A, new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Eor>(&mRegisters.A, new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Lsr>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetFlag>(&mRegisters.flags.Interrupt, false), make_shared<Eor>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Eor>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Lsr>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 6 */ {make_shared<Rts>(&mCPU, new Empty(), &mRegisters.sp), make_shared<Add>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Add>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Ror>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_shared<PullAcc>(&mRegisters.A, &mRegisters.sp, &mRegisters.flags), make_shared<Add>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_shared<Ror>(new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Jmp>(&mCPU, new Indirect(mCPU), &mRegisters.sp), make_shared<Add>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_shared<Ror>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 7 */ {make_shared<Bvs>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_shared<Add>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Add>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Ror>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetFlag>(&mRegisters.flags.Interrupt, true), make_shared<Add>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Add>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Ror>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 8 */ {nullptr, make_shared<Store>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Store>(new Implied(&mRegisters.Y), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.X), new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_shared<Dec>(new Implied(&mRegisters.Y), &mRegisters.flags), nullptr, make_shared<Transfer>(new Implied(&mRegisters.A), new Implied(&mRegisters.X), &mRegisters.flags), nullptr, make_shared<Store>(new Implied(&mRegisters.Y), new Absolute(mCPU), &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.X), new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 9 */ {make_shared<Bcc>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, make_shared<Store>(new Implied(&mRegisters.Y), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.X), new ZeroPageInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, make_shared<Transfer>(new Implied(&mRegisters.A), new Implied(&mRegisters.Y), &mRegisters.flags), make_shared<Store>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), make_shared<TransferToStack>(&mRegisters.X, &mRegisters.sp), nullptr, nullptr, make_shared<Store>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr},
            /* A */ {make_shared<Load>(new Implied(&mRegisters.Y), new Immediate, &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.X), new Immediate, &mRegisters.flags), nullptr, make_shared<Load>(new Implied(&mRegisters.Y), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.X), new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_shared<Transfer>(new Implied(&mRegisters.Y), new Implied(&mRegisters.A), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_shared<Transfer>(new Implied(&mRegisters.X), new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Load>(new Implied(&mRegisters.Y), new Absolute(mCPU), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.X), new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* B */ {make_shared<Bcs>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, make_shared<Load>(new Implied(&mRegisters.Y), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.X), new ZeroPageInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, make_shared<SetFlag>(&mRegisters.flags.Overflow, false), make_shared<Load>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), make_shared<TransferFromStack>(&mRegisters.X, &mRegisters.sp, &mRegisters.flags), nullptr, make_shared<Load>(new Implied(&mRegisters.Y), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(new Implied(&mRegisters.X), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr},
            /* C */ {make_shared<Cmp>(new Implied(&mRegisters.Y), new Immediate, &mRegisters.flags), make_shared<Cmp>(new Implied(&mRegisters.X), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Cmp>(new Implied(&mRegisters.Y), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Cmp>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Dec>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_shared<Inc>(new Implied(&mRegisters.Y), &mRegisters.flags), make_shared<Cmp>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_shared<Dec>(new Implied(&mRegisters.X), &mRegisters.flags), nullptr, make_shared<Cmp>(new Implied(&mRegisters.Y), new Absolute(mCPU), &mRegisters.flags), make_shared<Cmp>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_shared<Dec>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* D */ {make_shared<Bne>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_shared<Cmp>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Cmp>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Dec>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetFlag>(&mRegisters.flags.Decimal, false), make_shared<Cmp>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Cmp>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Dec>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* E */ {make_shared<Cmp>(new Implied(&mRegisters.X), new Immediate, &mRegisters.flags), make_shared<Sub>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Cmp>(new Implied(&mRegisters.X), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Sub>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_shared<Inc>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_shared<Inc>(new Implied(&mRegisters.X), &mRegisters.flags), make_shared<Sub>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_shared<Nop>(), nullptr, make_shared<Cmp>(new Implied(&mRegisters.X), new Absolute(mCPU), &mRegisters.flags), make_shared<Sub>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_shared<Inc>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* F */ {make_shared<Beq>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_shared<Sub>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Sub>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Inc>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetFlag>(&mRegisters.flags.Decimal, true), make_shared<Sub>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Sub>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Inc>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr}};
}

void Processor::loadFromFile(ifstream &file, size_t size)
{
    file.read(reinterpret_cast<char *>(&mCPU.rom), sizeof(CPU::rom) * size);
}

void Processor::doStep()
{
    mCPU.mPPURegs[0].PPUSTATUS.setBit(7, true);
    uint8_t buf = *mRegisters.pc;
    shared_ptr<IInstruction> iter = mInstructions[buf >> 0x4][buf % 0x10];
    cout << typeid(*iter.get()).name() << endl;
    iter->code(&mRegisters.pc);
    iter->execute();
    (mRegisters.pc)++;
}

bool Processor::eof() const
{
    return mRegisters.pc == &mCPU.rom[32767];
}
