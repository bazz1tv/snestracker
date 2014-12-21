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

#ifndef _memmap_h_
#define _memmap_h_

#include "snes9x.h"

#ifdef FAST_LSB_WORD_ACCESS
#define READ_WORD(s) (*(uint16 *) (s))
#define WRITE_WORD(s, d) (*(uint16 *) (s) = (d)
#else
#define READ_WORD(s) (*(uint8 *) (s) | (*((uint8 *) (s) + 1) << 8))
#define WRITE_WORD(s, d) *(uint8 *) (s) = (d), \
                         *((uint8 *) (s) + 1) = (d) >> 8
#endif

#if 0
#define MEMMAP_BLOCK_SIZE (0x1000)
#define MEMMAP_NUM_BLOCKS (0x1000000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_BLOCKS_PER_BANK (0x10000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_SHIFT 12
#define MEMMAP_MASK (MEMMAP_BLOCK_SIZE - 1)

class CMemory {
public:
    bool8 LoadROM (const char *);
    bool8 LoadSRAM (const char *);
    bool8 SaveSRAM (const char *);
    bool8 Init ();
    void  Deinit ();

    void WriteProtectROM ();
    void FixROMSpeed ();
    void MapRAM ();
    char *Safe (const char *);
    
    void LoROMMap ();
    void LoROM24MBSMap ();
    void HiROMMap ();
    void SuperFXROMMap ();
    void TalesROMMap ();
    void AlphaROMMap ();
    void SA1ROMMap ();
    bool8 AllASCII (uint8 *b, int size);
    int  ScoreHiROM (bool8 skip_header);
    int  ScoreLoROM (bool8 skip_header);
    void ApplyROMFixes ();
    const char *TVStandard ();
    const char *Speed ();
    const char *StaticRAMSize ();
    const char *MapType ();
    const char *MapMode ();
    const char *KartContents ();
    const char *Size ();
    const char *Headers ();
    const char *ROMID ();

    enum {
	MAP_PPU, MAP_CPU, MAP_DSP, MAP_LOROM_SRAM, MAP_HIROM_SRAM,
	MAP_NONE, MAP_DEBUG, MAP_BWRAM, MAP_BWRAM_BITMAP, MAP_BWRAM_BITMAP2,
	MAP_SA1RAM, MAP_LAST
    };
    enum { MAX_ROM_SIZE = 0xa00000 };
    
    uint8 *RAM;
    uint8 *ROM;
    uint8 *VRAM;
    uint8 *SRAM;
    uint8 *BWRAM;
    uint8 *FillRAM;
    bool8 HiROM;
    bool8 LoROM;
    uint16 SRAMMask;
    uint8 SRAMSize;
    uint8 *Map [MEMMAP_NUM_BLOCKS];
    uint8 *WriteMap [MEMMAP_NUM_BLOCKS];
    uint8 MemorySpeed [MEMMAP_NUM_BLOCKS];
    uint8 BlockIsRAM [MEMMAP_NUM_BLOCKS];
    uint8 BlockIsROM [MEMMAP_NUM_BLOCKS];
    char  ROMName [ROM_NAME_LEN];
    char  ROMId [5];
    uint8 ROMSpeed;
    uint8 ROMType;
    uint8 ROMSize;
    int32 ROMFramesPerSecond;
    int32 HeaderCount;
    uint32 CalculatedSize;
    uint32 CalculatedChecksum;
    uint32 ROMChecksum;
    uint32 ROMComplementChecksum;
    char ROMFilename [_MAX_PATH];
};

START_EXTERN_C
extern CMemory Memory;
extern uint8 *SRAM;
extern uint8 *ROM;
extern uint8 *RegRAM;
END_EXTERN_C

void S9xAutoSaveSRAM ();

#ifdef NO_INLINE_SET_GET
uint8 S9xGetByte (uint32 Address);
uint16 S9xGetWord (uint32 Address);
void S9xSetByte (uint8 Byte, uint32 Address);
void S9xSetWord (uint16 Byte, uint32 Address);
void S9xSetPCBase (uint32 Address);
uint8 *S9xGetMemPointer (uint32 Address);
uint8 *GetBasePointer (uint32 Address);
#else
#define INLINE inline
#include "getset.h"
#endif // NO_INLINE_SET_GET

#endif
#endif // _memmap_h_
