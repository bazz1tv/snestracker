/*
 * Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
 *
 * (c) Copyright 1996 - 2000 Gary Henderson (gary@daniver.demon.co.uk) and
 *                           Jerremy Koot (jkoot@snes9x.com)
 *
 * Super FX C emulator code 
 * (c) Copyright 1997 - 1999 Ivar (Ivar@snes9x.com) and
 *                           Gary Henderson.
 * Super FX assembler emulator code (c) Copyright 1998 zsKnight and _Demo_.
 *
 * DSP1 emulator code (c) Copyright 1998 Ivar, _Demo_ and Gary Henderson.
 * DOS port code contains the works of other authors. See headers in
 * individual files.
 *
 * Snes9x homepage: www.snes9x.com
 *
 * Permission to use, copy, modify and distribute Snes9x in both binary and
 * source form, for non-commercial purposes, is hereby granted without fee,
 * providing that this license information and copyright notice appear with
 * all copies and any derived work.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event shall the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Snes9x is freeware for PERSONAL USE only. Commercial users should
 * seek permission of the copyright holders first. Commercial use includes
 * charging money for Snes9x or software derived from Snes9x.
 *
 * The copyright holders request that bug fixes and improvements to the code
 * should be forwarded to them so everyone can benefit from the modifications
 * in future versions.
 *
 * Super NES and Super Nintendo Entertainment System are trademarks of
 * Nintendo Co., Limited and its subsidiary companies.
 */

#ifndef _SPC700_H_
#define _SPC700_H_

#define Carry       1
#define Zero        2
#define Interrupt   4
#define HalfCarry   8
#define BreakFlag  16
#define DirectPageFlag 32
#define Overflow   64
#define Negative  128

#define APUClearCarry() (IAPU._Carry = 0)
#define APUSetCarry() (IAPU._Carry = 1)
#define APUSetInterrupt() (APURegisters.P |= Interrupt)
#define APUClearInterrupt() (APURegisters.P &= ~Interrupt)
#define APUSetHalfCarry() (APURegisters.P |= HalfCarry)
#define APUClearHalfCarry() (APURegisters.P &= ~HalfCarry)
#define APUSetBreak() (APURegisters.P |= BreakFlag)
#define APUClearBreak() (APURegisters.P &= ~BreakFlag)
#define APUSetDirectPage() (APURegisters.P |= DirectPageFlag)
#define APUClearDirectPage() (APURegisters.P &= ~DirectPageFlag)
#define APUSetOverflow() (IAPU._Overflow = 1)
#define APUClearOverflow() (IAPU._Overflow = 0)

#define APUCheckZero() (IAPU._Zero == 0)
#define APUCheckCarry() (IAPU._Carry)
#define APUCheckInterrupt() (APURegisters.P & Interrupt)
#define APUCheckHalfCarry() (APURegisters.P & HalfCarry)
#define APUCheckBreak() (APURegisters.P & BreakFlag)
#define APUCheckDirectPage() (APURegisters.P & DirectPageFlag)
#define APUCheckOverflow() (IAPU._Overflow)
#define APUCheckNegative() (IAPU._Zero & 0x80)

#define APUClearFlags(f) (APURegisters.P &= ~(f))
#define APUSetFlags(f)   (APURegisters.P |=  (f))
#define APUCheckFlag(f)  (APURegisters.P &   (f))

typedef union
{
#ifdef LSB_FIRST
    struct { uint8 A, Y; } B;
#else
    struct { uint8 Y, A; } B;
#endif
    uint16 W;
} YAndA;

struct SAPURegisters{
    uint8  P;
    YAndA YA;
    uint8  X;
    uint8  S;
    uint16  PC;
};

EXTERN_C struct SAPURegisters APURegisters;
//EXTERN_C void report_memread2(unsigned short address, unsigned char val);
#include "report.h"

// Needed by ILLUSION OF GAIA
#define ONE_APU_CYCLE 14

// Needed by all games written by the software company called Human
#define ONE_APU_CYCLE_HUMAN 17

#ifdef DEBUGGER
#define APU_EXECUTE1() \
{ \
    if (APU.Flags & TRACE_FLAG) \
	S9xTraceAPU ();\
    APU.Cycles += S9xAPUCycles [*IAPU.PC]; \
    (*S9xApuOpcodes[*IAPU.PC]) (); \
}
#else
#define APU_EXECUTE1() \
{ \
    APU.Cycles += S9xAPUCycles [*IAPU.PC]; \
	report_memread2(APURegisters.PC, *IAPU.PC); \
    (*S9xApuOpcodes[*IAPU.PC]) (); \
}
#endif

#define APU_EXECUTE() \
if (IAPU.APUExecuting) \
{\
    while (APU.Cycles <= CPU.Cycles) \
	APU_EXECUTE1(); \
}

#endif
