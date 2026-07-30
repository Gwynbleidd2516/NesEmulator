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
            /* 0 */ {make_shared<Brk>(mCPU, make_shared<Empty>(), &mRegisters.sp, &mRegisters.flags),
                     make_shared<Ora>(&mRegisters.A, make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags),
                     nullptr, nullptr, nullptr,
                     make_shared<Ora>(&mRegisters.A, make_shared<ZeroPage>(mCPU), &mRegisters.flags),
                     make_shared<Asl>(make_shared<ZeroPage>(mCPU), &mRegisters.flags),
                     nullptr,
                     make_shared<PushProcStatus>(&mRegisters.flags, &mRegisters.sp),
                     make_shared<Ora>(&mRegisters.A, make_shared<Immediate>(), &mRegisters.flags),
                     make_shared<Asl>(make_shared<Implied>(&mRegisters.A), &mRegisters.flags),
                     nullptr, nullptr,
                     make_shared<Ora>(&mRegisters.A, make_shared<Absolute>(mCPU), &mRegisters.flags),
                     make_shared<Asl>(make_shared<Absolute>(mCPU), &mRegisters.flags),
                     nullptr},
            /* 1 */ {make_shared<Bpl>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<Ora>(&mRegisters.A, make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Ora>(&mRegisters.A, make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Asl>(make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetCarry>(&mRegisters.flags, false), make_shared<Ora>(&mRegisters.A, make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Ora>(&mRegisters.A, make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Asl>(make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 2 */ {make_shared<Jsr>(mCPU, make_shared<Absolute>(mCPU), &mRegisters.sp), make_shared<And>(&mRegisters.A, make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Bit>(&mRegisters.A, make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<And>(&mRegisters.A, make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Rol>(make_shared<ZeroPage>(mCPU), &mRegisters.flags), nullptr, make_shared<PullProcStatus>(&mRegisters.flags, &mRegisters.sp), make_shared<And>(&mRegisters.A, make_shared<Immediate>(), &mRegisters.flags), make_shared<Rol>(make_shared<Implied>(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Bit>(&mRegisters.A, make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<And>(&mRegisters.A, make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Rol>(make_shared<Absolute>(mCPU), &mRegisters.flags), nullptr},
            /* 3 */ {make_shared<Bmi>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<And>(&mRegisters.A, make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<And>(&mRegisters.A, make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Rol>(make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetCarry>(&mRegisters.flags, true), make_shared<And>(&mRegisters.A, make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<And>(&mRegisters.A, make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Rol>(make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 4 */ {make_shared<Rti>(mCPU, &mRegisters.sp, &mRegisters.flags), make_shared<Eor>(&mRegisters.A, make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Eor>(&mRegisters.A, make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Lsr>(make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<PushAcc>(&mRegisters.A, &mRegisters.sp), make_shared<Eor>(&mRegisters.A, make_shared<Immediate>(), &mRegisters.flags), make_shared<Lsr>(make_shared<Implied>(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Jmp>(mCPU, make_shared<Absolute>(mCPU), &mRegisters.sp), make_shared<Eor>(&mRegisters.A, make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Lsr>(make_shared<Absolute>(mCPU), &mRegisters.flags), nullptr},
            /* 5 */ {make_shared<Bvc>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<Eor>(&mRegisters.A, make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Eor>(&mRegisters.A, make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Lsr>(make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetInterrupt>(&mRegisters.flags, false), make_shared<Eor>(&mRegisters.A, make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Eor>(&mRegisters.A, make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Lsr>(make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 6 */ {make_shared<Rts>(mCPU, make_shared<Empty>(), &mRegisters.sp), make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Ror>(make_shared<ZeroPage>(mCPU), &mRegisters.flags), nullptr, make_shared<PullAcc>(&mRegisters.A, &mRegisters.sp, &mRegisters.flags), make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<Immediate>(), &mRegisters.flags), make_shared<Ror>(make_shared<Implied>(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Jmp>(mCPU, make_shared<Indirect>(mCPU), &mRegisters.sp), make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Ror>(make_shared<Absolute>(mCPU), &mRegisters.flags), nullptr},
            /* 7 */ {make_shared<Bvs>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Ror>(make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetInterrupt>(&mRegisters.flags, true), make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Add>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Ror>(make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* 8 */ {nullptr, make_shared<Store>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Store>(make_shared<Implied>(&mRegisters.Y), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.X), make_shared<ZeroPage>(mCPU), &mRegisters.flags), nullptr, make_shared<Dec>(make_shared<Implied>(&mRegisters.Y), &mRegisters.flags), nullptr, make_shared<Transfer>(make_shared<Implied>(&mRegisters.A), make_shared<Implied>(&mRegisters.X), &mRegisters.flags), nullptr, make_shared<Store>(make_shared<Implied>(&mRegisters.Y), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.A), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.X), make_shared<Absolute>(mCPU), &mRegisters.flags), nullptr},
            /* 9 */ {make_shared<Bcc>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, make_shared<Store>(make_shared<Implied>(&mRegisters.Y), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.X), make_shared<ZeroPageInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, make_shared<Transfer>(make_shared<Implied>(&mRegisters.A), make_shared<Implied>(&mRegisters.Y), &mRegisters.flags), make_shared<Store>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), make_shared<TransferToStack>(&mRegisters.X, &mRegisters.sp), nullptr, nullptr, make_shared<Store>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr},
            /* A */ {make_shared<Load>(make_shared<Implied>(&mRegisters.Y), make_shared<Immediate>(), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.X), make_shared<Immediate>(), &mRegisters.flags), nullptr, make_shared<Load>(make_shared<Implied>(&mRegisters.Y), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.X), make_shared<ZeroPage>(mCPU), &mRegisters.flags), nullptr, make_shared<Transfer>(make_shared<Implied>(&mRegisters.Y), make_shared<Implied>(&mRegisters.A), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<Immediate>(), &mRegisters.flags), make_shared<Transfer>(make_shared<Implied>(&mRegisters.X), make_shared<Implied>(&mRegisters.A), &mRegisters.flags), nullptr, make_shared<Load>(make_shared<Implied>(&mRegisters.Y), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.X), make_shared<Absolute>(mCPU), &mRegisters.flags), nullptr},
            /* B */ {make_shared<Bcs>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, make_shared<Load>(make_shared<Implied>(&mRegisters.Y), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.X), make_shared<ZeroPageInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, make_shared<SetOverflow>(&mRegisters.flags, false), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), make_shared<TransferFromStack>(&mRegisters.X, &mRegisters.sp, &mRegisters.flags), nullptr, make_shared<Load>(make_shared<Implied>(&mRegisters.Y), make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Load>(make_shared<Implied>(&mRegisters.X), make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr},
            /* C */ {make_shared<Cmp>(make_shared<Implied>(&mRegisters.Y), make_shared<Immediate>(), &mRegisters.flags), make_shared<Cmp>(make_shared<Implied>(&mRegisters.X), make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Cmp>(make_shared<Implied>(&mRegisters.Y), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Cmp>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Dec>(make_shared<ZeroPage>(mCPU), &mRegisters.flags), nullptr, make_shared<Inc>(make_shared<Implied>(&mRegisters.Y), &mRegisters.flags), make_shared<Cmp>(make_shared<Implied>(&mRegisters.A), make_shared<Immediate>(), &mRegisters.flags), make_shared<Dec>(make_shared<Implied>(&mRegisters.X), &mRegisters.flags), nullptr, make_shared<Cmp>(make_shared<Implied>(&mRegisters.Y), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Cmp>(make_shared<Implied>(&mRegisters.A), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Dec>(make_shared<Absolute>(mCPU), &mRegisters.flags), nullptr},
            /* D */ {make_shared<Bne>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<Cmp>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Cmp>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Dec>(make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetDecimal>(&mRegisters.flags, false), make_shared<Cmp>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Cmp>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Dec>(make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr},
            /* E */ {make_shared<Cmp>(make_shared<Implied>(&mRegisters.X), make_shared<Immediate>(), &mRegisters.flags), make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectX>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, nullptr, make_shared<Cmp>(make_shared<Implied>(&mRegisters.X), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPage>(mCPU), &mRegisters.flags), make_shared<Inc>(make_shared<ZeroPage>(mCPU), &mRegisters.flags), nullptr, make_shared<Inc>(make_shared<Implied>(&mRegisters.X), &mRegisters.flags), make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<Immediate>(), &mRegisters.flags), make_shared<Nop>(), nullptr, make_shared<Cmp>(make_shared<Implied>(&mRegisters.X), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<Absolute>(mCPU), &mRegisters.flags), make_shared<Inc>(make_shared<Absolute>(mCPU), &mRegisters.flags), nullptr},
            /* F */ {make_shared<Beq>(&mRegisters.pc, make_shared<Immediate>(), &mRegisters.flags), make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<IndirectY>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Inc>(make_shared<ZeroPageInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr, make_shared<SetDecimal>(&mRegisters.flags, true), make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.Y), &mRegisters.flags), nullptr, nullptr, nullptr, make_shared<Sub>(make_shared<Implied>(&mRegisters.A), make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), make_shared<Inc>(make_shared<AbsoluteInd>(mCPU, &mRegisters.X), &mRegisters.flags), nullptr}};
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
