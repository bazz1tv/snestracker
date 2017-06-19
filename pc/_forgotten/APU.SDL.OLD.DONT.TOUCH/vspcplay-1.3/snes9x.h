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

#ifndef _SNES9X_H_
#define _SNES9X_H_

#define VERSION "1.29"

#include <stdio.h>
#include <stdlib.h>

//#ifdef _WIN32
//#include "..\Snes9X.h"
//#include "..\zlib\zlib.h"
//#endif

#include "port.h"
/* #include "65c816.h" */
/* #include "messages.h" */

#if defined(USE_GLIDE) && !defined(GFX_MULTI_FORMAT)
#define GFX_MULTI_FORMAT
#endif

#define ROM_NAME_LEN 23

#ifdef ZLIB
#ifndef _WIN32
#include "zlib.h"
#endif
#define STREAM gzFile
#define READ_STREAM(p,l,s) gzread (s,p,l)
#define WRITE_STREAM(p,l,s) gzwrite (s,p,l)
#define OPEN_STREAM(f,m) gzopen (f,m)
#define CLOSE_STREAM(s) gzclose (s)
#else
#define STREAM FILE *
#define READ_STREAM(p,l,s) fread (p,1,l,s)
#define WRITE_STREAM(p,l,s) fwrite (p,1,l,s)
#define OPEN_STREAM(f,m) fopen (f,m)
#define CLOSE_STREAM(s) fclose (s)
#endif


/* SNES screen width and height */
#define SNES_WIDTH		256
#define SNES_HEIGHT		224
#define SNES_HEIGHT_EXTENDED	239
#define IMAGE_WIDTH		(Settings.SupportHiRes ? SNES_WIDTH * 2 : SNES_WIDTH)
#define IMAGE_HEIGHT		(Settings.SupportHiRes ? SNES_HEIGHT_EXTENDED * 2 : SNES_HEIGHT_EXTENDED)

#define SNES_MAX_NTSC_VCOUNTER  262
#define SNES_MAX_PAL_VCOUNTER   312
#define SNES_HCOUNTER_MAX	341
#define SPC700_TO_65C816_RATIO	2
#define AUTO_FRAMERATE		200

#define SNES_SCANLINE_TIME (63.49e-6)

#ifdef VAR_CYCLES
#define SNES_CYCLES_PER_SCANLINE ((uint16) (63.5e-6 / (1 / 3580000.0)) * 6)
#else
#define SNES_CYCLES_PER_SCANLINE ((uint16) (63.5e-6 / (1 / 3580000.0)))
#endif

#define SCANLINE_FREQUENCY (15748.0315)

#define SNES_TR_MASK	    (1 << 4)
#define SNES_TL_MASK	    (1 << 5)
#define SNES_X_MASK	    (1 << 6)
#define SNES_A_MASK	    (1 << 7)
#define SNES_RIGHT_MASK	    (1 << 8)
#define SNES_LEFT_MASK	    (1 << 9)
#define SNES_DOWN_MASK	    (1 << 10)
#define SNES_UP_MASK	    (1 << 11)
#define SNES_START_MASK	    (1 << 12)
#define SNES_SELECT_MASK    (1 << 13)
#define SNES_Y_MASK	    (1 << 14)
#define SNES_B_MASK	    (1 << 15)

enum {
    SNES_MULTIPLAYER5,
    SNES_JOYPAD,
    SNES_MOUSE_SWAPPED,
    SNES_MOUSE,
    SNES_SUPERSCOPE,
    SNES_MAX_CONTROLLER_OPTIONS
};

#define DEBUG_MODE_FLAG	    (1 << 0)
#define TRACE_FLAG	    (1 << 1)
#define SINGLE_STEP_FLAG    (1 << 2)
#define BREAK_FLAG	    (1 << 3)
#define SCAN_KEYS_FLAG	    (1 << 4)
#define SAVE_SNAPSHOT_FLAG  (1 << 5)
#define DELAYED_NMI_FLAG    (1 << 6)
#define NMI_FLAG	    (1 << 7)
#define PROCESS_SOUND_FLAG  (1 << 8)
#define FRAME_ADVANCE_FLAG  (1 << 9)
#define DELAYED_NMI_FLAG2   (1 << 10)
#define IRQ_PENDING_FLAG    (1 << 11)

#ifdef VAR_CYCLES
#define ONE_CYCLE 6
#define TWO_CYCLES 12
#else
#define ONE_CYCLE 1
#define TWO_CYCLES 2
#endif

struct SCPUState{
    uint32  Flags;
    bool8   BranchSkip;
    bool8   NMIActive;
    bool8   IRQActive;
    bool8   WaitingForInterrupt;
    bool8   InDMA;
    uint8   WhichEvent;
    uint8   *PC;
    uint8   *PCBase;
    uint8   *PCAtOpcodeStart;
    uint8   *WaitAddress;
    uint32  WaitCounter;
    long   Cycles;
    long   NextEvent;
    long   V_Counter;
    long   MemSpeed;
    long   MemSpeedx2;
    long   FastROMSpeed;
    uint32 AutoSaveTimer;
    bool8  SRAMModified;
};

#define HBLANK_START_EVENT 0
#define HBLANK_END_EVENT 1
#define HTIMER_EVENT 2
#define NO_EVENT 3

struct SSettings{
    // CPU options
    bool8  APUEnabled;
    bool8  Shutdown;
    uint8  SoundSkipMethod;
    long   H_Max;
    long   HBlankStart;
    uint32 SPCTo65c816Ratio;
    bool8  DisableIRQ;
    uint8  Paused;
    bool8  BattleToadsNMIHack;

    // Tracing options
    bool8  TraceDMA;
    bool8  TraceHDMA;
    bool8  TraceVRAM;
    bool8  TraceUnknownRegisters;
    bool8  TraceDSP;

    // Joystick options
    bool8  SwapJoypads;
    bool8  JoystickEnabled;

    // ROM timing options (see also H_Max above)
    bool8  ForcePAL;
    bool8  ForceNTSC;
    bool8  PAL;
    uint32 FrameTimePAL;
    uint32 FrameTimeNTSC;
    uint32 FrameTime;
    uint32 SkipFrames;

    // ROM image options
    bool8  ForceLoROM;
    bool8  ForceHiROM;
    bool8  ForceHeader;
    bool8  ForceNoHeader;
    bool8  ForceInterleaved;
    bool8  ForceInterleaved2;
    bool8  ForceNotInterleaved;

    // Peripherial options
    bool8  ForceSuperFX;
    bool8  ForceNoSuperFX;
    bool8  ForceDSP1;
    bool8  ForceNoDSP1;
    bool8  ForceSA1;
    bool8  ForceNoSA1;
    bool8  MultiPlayer5;
    bool8  Mouse;
    bool8  SuperScope;
    uint32 ControllerOption;
    
    bool8  MultiPlayer5Master;
    bool8  SuperScopeMaster;
    bool8  MouseMaster;
    bool8  SuperFX;
    bool8  DSP1Master;
    bool8  SA1;

    // Sound options
    bool8  TraceSoundDSP;
    uint8  SoundPlaybackRate;
    bool8  Stereo;
	bool8  ReverseStereo;
    int    SoundBufferSize;
    bool8  SoundEnvelopeHeightReading;
    bool8  DisableSoundEcho;
    bool8  DisableSampleCaching;
    bool8  DisableMasterVolume;
    bool8  SoundSync;
    bool8  InterpolatedSound;
    bool8  ThreadSound;
	uint8  AltSampleDecode;
   
    // Graphics options
    bool8  SixteenBit;
    bool8  Transparency;
    bool8  SupportHiRes;

    // SNES graphics options
    bool8  BGLayering;
    bool8  DisableGraphicWindows;
    bool8  ForceTransparency;
    bool8  ForceNoTransparency;
    bool8  DisableHDMA;
    bool8  DisplayFrameRate;

    // Others
    bool8  NetPlay;
    char   ServerName [128];
    int    Port;
#ifdef USE_GLIDE
    bool8  GlideEnable;
#endif
    bool8  DaffyDuckNMIHack;
    bool8  StarfoxHack;
    bool8  WinterGold;
    bool8  Seiken3;
    bool8  Stargate;
    bool8  EnableExtraNoise;
    int32  AutoSaveDelay; // Time in seconds before S-RAM auto-saved if modified.
    bool8  ApplyCheats;
};

START_EXTERN_C
extern struct SSettings Settings;
extern struct SCPUState CPU;
extern char String [513];

void S9xExit ();
void S9xMessage (int type, int number, const char *message);
END_EXTERN_C

#endif
