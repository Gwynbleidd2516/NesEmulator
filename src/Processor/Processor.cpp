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
#include "SetNegative.h"
#include "SetOverflow.h"
#include "SetBreak.h"
#include "SetDecimal.h"
#include "SetInterrupt.h"
#include "SetZero.h"
#include "SetCarry.h"
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

#include "Logs.h"

Processor::Processor()
{
}

void Processor::launch()
{
    for (; mIsRunning->load(); ++(mRegisters.pc))
    {
        if (mNmiClock.getElapsedTime().asSeconds() >= 1.0f / 60.0f)
        {
            nmi();
            mNmiClock.restart();
        }
        mCPU->memoryMap.mPPURegs->ppustatus.vblank = true;
#ifdef DO_LOGS
        Logs::GetInstance().registers->info("{}", mRegisters);
#endif
        IInstruction *iter = mInstructions[*mRegisters.pc >> 0x4][*mRegisters.pc % 0x10].get();
        iter->code(&mRegisters.pc);
        iter->execute();
#ifdef DO_LOGS
        Logs::GetInstance().instruction = typeid(*iter).name() + 6;
        Logs::GetInstance().print_pc_status();
#endif
    }
}

bool Processor::eof() const
{
    return false;
}

void Processor::reset()
{
#ifdef DO_LOGS
    Logs::GetInstance().pc_status->info("reset vector is launched");
#endif
    mRegisters.pc = &mCPU->at(mCPU->memoryMap.resetVector.raw);
    mNmiClock.start();
}

void Processor::nmi()
{
    if (mCPU->memoryMap.mPPURegs->ppuctrl.nmi_enable == 1)
    {
#ifdef DO_LOGS
        Logs::GetInstance().pc_status->info("nmi vector is launched");
#endif
        mCPU->memoryMap.mPPURegs->ppuctrl.nmi_enable = 0;
        mCPU->memoryMap.mPPURegs->ppustatus.vblank = 0;
        Struct16_t buf;
        buf.raw = mRegisters.pc - (uint8_t *)&mCPU->memoryMap;
        *mRegisters.sp = buf.h;
        mRegisters.sp++;
        *mRegisters.sp = buf.l;
        mRegisters.sp++;
        mRegisters.flags.Break = false;
        *mRegisters.sp = mRegisters.flags.raw;
        mRegisters.sp++;

        mRegisters.pc = &mCPU->at(mCPU->memoryMap.nmiVector.raw);
    }
}

void Processor::setCPU(CPU *cpu)
{
    mCPU = cpu;
    mRegisters.sp = &(mCPU->memoryMap.mMirror[0].stack[0]);
    mInstructions =
        {
            //          0        1        2
            /* 0 */ {make_unique<Brk>(mCPU, new Empty, &mRegisters.sp, &mRegisters.flags),
                     make_unique<Ora>(&mRegisters.A, new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr, nullptr,
                     make_unique<Ora>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags),
                     make_unique<Asl>(new ZeroPage(mCPU), &mRegisters.flags),
                     nullptr,
                     make_unique<PushProcStatus>(&mRegisters.flags, &mRegisters.sp),
                     make_unique<Ora>(&mRegisters.A, new Immediate(), &mRegisters.flags),
                     make_unique<Asl>(new Implied(&mRegisters.A), &mRegisters.flags),
                     nullptr, nullptr,
                     make_unique<Ora>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags),
                     make_unique<Asl>(new Absolute(mCPU), &mRegisters.flags),
                     nullptr},
            /* 1 */ {make_unique<Bpl>(&mRegisters.pc, new Immediate(), &mRegisters.flags), make_unique<Ora>(&mRegisters.A, new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Ora>(&mRegisters.A, new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Asl>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_unique<SetCarry>(&mRegisters.flags, false), make_unique<Ora>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Ora>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Asl>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 2 */ {make_unique<Jsr>(mCPU, new Absolute(mCPU), &mRegisters.sp), make_unique<And>(&mRegisters.A, new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_unique<Bit>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags), make_unique<And>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags), make_unique<Rol>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_unique<PullProcStatus>(&mRegisters.flags, &mRegisters.sp), make_unique<And>(&mRegisters.A, new Immediate(), &mRegisters.flags), make_unique<Rol>(new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_unique<Bit>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags), make_unique<And>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags), make_unique<Rol>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 3 */ {make_unique<Bmi>(&mRegisters.pc, new Immediate(), &mRegisters.flags), make_unique<And>(&mRegisters.A, new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<And>(&mRegisters.A, new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Rol>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_unique<SetCarry>(&mRegisters.flags, true), make_unique<And>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<And>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Rol>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 4 */ {make_unique<Rti>(mCPU, &mRegisters.sp, &mRegisters.flags), make_unique<Eor>(&mRegisters.A, new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Eor>(&mRegisters.A, new ZeroPage(mCPU), &mRegisters.flags), make_unique<Lsr>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_unique<PushAcc>(&mRegisters.A, &mRegisters.sp), make_unique<Eor>(&mRegisters.A, new Immediate(), &mRegisters.flags), make_unique<Lsr>(new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_unique<Jmp>(mCPU, new Absolute(mCPU), &mRegisters.sp), make_unique<Eor>(&mRegisters.A, new Absolute(mCPU), &mRegisters.flags), make_unique<Lsr>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 5 */ {make_unique<Bvc>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_unique<Eor>(&mRegisters.A, new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Eor>(&mRegisters.A, new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Lsr>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_unique<SetInterrupt>(&mRegisters.flags, false), make_unique<Eor>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Eor>(&mRegisters.A, new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Lsr>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 6 */ {make_unique<Rts>(mCPU, new Empty(), &mRegisters.sp), make_unique<Add>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Add>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Ror>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_unique<PullAcc>(&mRegisters.A, &mRegisters.sp, &mRegisters.flags), make_unique<Add>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_unique<Ror>(new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_unique<Jmp>(mCPU, new Indirect(mCPU), &mRegisters.sp), make_unique<Add>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_unique<Ror>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 7 */ {make_unique<Bvs>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_unique<Add>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Add>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Ror>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_unique<SetInterrupt>(&mRegisters.flags, true), make_unique<Add>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Add>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Ror>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 8 */ {nullptr, make_unique<Store>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_unique<Store>(new Implied(&mRegisters.Y), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.X), new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_unique<Dec>(new Implied(&mRegisters.Y), &mRegisters.flags), nullptr, make_unique<Transfer>(new Implied(&mRegisters.A), new Implied(&mRegisters.X), &mRegisters.flags), nullptr, make_unique<Store>(new Implied(&mRegisters.Y), new Absolute(mCPU), &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.X), new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* 9 */ {make_unique<Bcc>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, make_unique<Store>(new Implied(&mRegisters.Y), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.X), new ZeroPageInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, make_unique<Transfer>(new Implied(&mRegisters.A), new Implied(&mRegisters.Y), &mRegisters.flags), make_unique<Store>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), make_unique<TransferToStack>(&mRegisters.X, &mRegisters.sp), nullptr, nullptr, make_unique<Store>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr},
            /* A */ {make_unique<Load>(new Implied(&mRegisters.Y), new Immediate, &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.X), new Immediate, &mRegisters.flags), nullptr, make_unique<Load>(new Implied(&mRegisters.Y), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.X), new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_unique<Transfer>(new Implied(&mRegisters.Y), new Implied(&mRegisters.A), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_unique<Transfer>(new Implied(&mRegisters.X), new Implied(&mRegisters.A), &mRegisters.flags), nullptr, make_unique<Load>(new Implied(&mRegisters.Y), new Absolute(mCPU), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.X), new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* B */ {make_unique<Bcs>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, make_unique<Load>(new Implied(&mRegisters.Y), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.X), new ZeroPageInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, make_unique<SetOverflow>(&mRegisters.flags, false), make_unique<Load>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), make_unique<TransferFromStack>(&mRegisters.X, &mRegisters.sp, &mRegisters.flags), nullptr, make_unique<Load>(new Implied(&mRegisters.Y), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Load>(new Implied(&mRegisters.X), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr},
            /* C */ {make_unique<Cmp>(new Implied(&mRegisters.Y), new Immediate, &mRegisters.flags), make_unique<Cmp>(new Implied(&mRegisters.X), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_unique<Cmp>(new Implied(&mRegisters.Y), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Cmp>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Dec>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_unique<Inc>(new Implied(&mRegisters.Y), &mRegisters.flags), make_unique<Cmp>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_unique<Dec>(new Implied(&mRegisters.X), &mRegisters.flags), nullptr, make_unique<Cmp>(new Implied(&mRegisters.Y), new Absolute(mCPU), &mRegisters.flags), make_unique<Cmp>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_unique<Dec>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* D */ {make_unique<Bne>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_unique<Cmp>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Cmp>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Dec>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_unique<SetDecimal>(&mRegisters.flags, false), make_unique<Cmp>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Cmp>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Dec>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* E */ {make_unique<Cmp>(new Implied(&mRegisters.X), new Immediate, &mRegisters.flags), make_unique<Sub>(new Implied(&mRegisters.A), new IndirectX(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_unique<Cmp>(new Implied(&mRegisters.X), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Sub>(new Implied(&mRegisters.A), new ZeroPage(mCPU), &mRegisters.flags), make_unique<Inc>(new ZeroPage(mCPU), &mRegisters.flags), nullptr, make_unique<Inc>(new Implied(&mRegisters.X), &mRegisters.flags), make_unique<Sub>(new Implied(&mRegisters.A), new Immediate, &mRegisters.flags), make_unique<Nop>(), nullptr, make_unique<Cmp>(new Implied(&mRegisters.X), new Absolute(mCPU), &mRegisters.flags), make_unique<Sub>(new Implied(&mRegisters.A), new Absolute(mCPU), &mRegisters.flags), make_unique<Inc>(new Absolute(mCPU), &mRegisters.flags), nullptr},
            /* F */ {make_unique<Beq>(&mRegisters.pc, new Immediate, &mRegisters.flags), make_unique<Sub>(new Implied(&mRegisters.A), new IndirectY(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Sub>(new Implied(&mRegisters.A), new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Inc>(new ZeroPageInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_unique<SetDecimal>(&mRegisters.flags, true), make_unique<Sub>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_unique<Sub>(new Implied(&mRegisters.A), new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), make_unique<Inc>(new AbsoluteInd(mCPU, &mRegisters.X), &mRegisters.flags), nullptr}};
    mCPU->memoryMap.mPPURegs->ppuctrl.nmi_enable = false;
}

void Processor::setPPU(PPU *ppu)
{
    mCPU->ppu = ppu;
}

void Processor::setHeader(Header h)
{
    mHeader = h;
}

void Processor::setIsRunningAtomic(atomic<bool> *atm)
{
    mIsRunning = atm;
}
