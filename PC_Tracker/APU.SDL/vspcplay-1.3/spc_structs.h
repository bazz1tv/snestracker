#ifndef _spc_structs_h__
#define _spc_structs_h__

#include "port.h"

struct SIAPU
{
    uint8  *PC;
    uint8  *RAM;
    uint8  *DirectPage;
    bool8  APUExecuting;
    uint8  Bit;
    uint32 Address;
    uint8  *WaitAddress1;
    uint8  *WaitAddress2;
    uint32 WaitCounter;
    uint8  *ShadowRAM;
    uint8  *CachedSamples;
    uint8  _Carry;
    uint8  _Zero;
    uint8  _Overflow;
    uint32 TimerErrorCounter;
    uint32 Scanline;
    int32  OneCycle;
    int32  TwoCycles;
};

struct SAPU
{
    int32  Cycles;
    bool8  ShowROM;
    uint8  Flags;
    uint8  KeyedChannels;
    uint8  OutPorts [4];
    uint8  DSP [0x80];
    uint8  ExtraRAM [64];
    uint16 Timer [3];
    uint16 TimerTarget [3];
    bool8  TimerEnabled [3];
    bool8  TimerValueWritten [3];
};

#endif

