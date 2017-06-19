;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     If you use my code, please share your creations with me
;-     as I am always curious :)
;------------------------------------------------------------------------


;----------------------------------------------------------------------------
; InitSNES -- my "standard" initialization of SNES memory and registers
;----------------------------------------------------------------------------
.MACRO InitSNES
  sei                     ;disable interrupts
  clc                     ;switch to native mode
  xce

  REP #$38		; mem/A = 16 bit, X/Y = 16 bit
			;decimal mode off

  LDX #$1FFF	;Setup the stack
  TXS			;Transfer Index X to Stack Pointer Register

  ;do the rest of the initialization in a routine
  JSL $008000

  SEP #$20		; mem/A = 8 bit
.ENDM

;----------------------------------------------------------------------------

.BANK 0 SLOT 0
.ORG 0
.SECTION "InitializeSNESCode" FORCE

InitializeSNES:
  PHK			;set Data Bank = Program Bank
  PLB

  LDA #$0000	;set Direct Page = $0000
  TCD			;Transfer Accumulator to Direct Register

  LDX $1FFD		;we clear all the mem at one point ...
  STX $4372  	;so save the return address in a place that won't get overwritten
  LDX $1FFF
  STX $4374

  SEP #$20		; mem/A = 8 bit
  REP #$10

  LDA #$8F
  STA $2100		;turn screen off for now, set brightness to normal

  LDX #$2101
_Loop00:		;regs $2101-$210C
  STZ $00,X		;set Sprite,Character,Tile sizes to lowest, and set addresses to $0000
  INX
  CPX #$210D
  BNE _Loop00

_Loop01:		;regs $210D-$2114
  STZ $00,X		;Set all BG scroll values to $0000
  STZ $00,X
  INX
  CPX #$2115
  BNE _Loop01

  LDA #$80		;reg $2115
  STA $2115		; Initialize VRAM transfer mode to word-access, increment by 1

  STZ $2116		;regs $2117-$2117
  STZ $2117		;VRAM address = $0000

			;reg $2118-$2119
			;VRAM write register... don't need to initialize

  STZ $211A		;clear Mode7 setting

  LDX #$211B
_Loop02:		;regs $211B-$2120
  STZ $00,X		;clear out the Mode7 matrix values
  STZ $00,X
  INX
  CPX #$2121
  BNE _Loop02

			;reg $2121 - Color address, doesn't need initilaizing
			;reg $2122 - Color data, is initialized later

  LDX #$2123
_Loop03:		;regs $2123-$2133
  STZ $00,X		;turn off windows, main screens, sub screens, color addition,
  INX			;fixed color = $00, no super-impose (external synchronization),
  CPX #$2134	;no interlaced mode, normal resolution
  BNE _Loop03

			;regs $2134-$2136  - multiplication result, no initialization needed
			;reg $2137 - software H/V latch, no initialization needed
			;reg $2138 - Sprite data read, no initialization needed
			;regs $2139-$213A  - VRAM data read, no initialization needed
			;reg $213B - Color RAM data read, no initialization needed
			;regs $213C-$213D  - H/V latched data read, no initialization needed

  STZ $213E		;reg $213E - might not be necesary, but selects PPU master/slave mode
			;reg $213F - PPU status flag, no initialization needed

			;reg $2140-$2143 - APU communication regs, no initialization required

			;reg $2180  -  read/write WRAM register, no initialization required
			;reg $2181-$2183  -  WRAM address, no initialization required

			;reg $4016-$4017  - serial JoyPad read registers, no need to initialize


  STZ $4200		;reg $4200  - disable timers, NMI,and auto-joyread

  LDA #$FF
  STA $4201		;reg $4201  - programmable I/O write port, initalize to allow reading at in-port

			;regs $4202-$4203  - multiplication registers, no initialization required
			;regs $4204-$4206  - division registers, no initialization required

			;regs $4207-$4208  - Horizontal-IRQ timer setting, since we disabled this, it is OK to not init
			;regs $4209-$420A  - Vertical-IRQ timer setting, since we disabled this, it is OK to not init

  STZ $420B		;reg $420B  - turn off all general DMA channels
  STZ $420C		;reg $420C  - turn off all H-MA channels

  STZ $420D		;reg $420D  - ROM access time to slow (2.68Mhz)

  LDA $4210		;reg $4210  - NMI status, reading resets

			;reg $4211  - IRQ status, no need to initialize
			;reg $4212  - H/V blank and JoyRead status, no need to initialize
			;reg $4213  - programmable I/O inport, no need to initialize

			;reg $4214-$4215  - divide results, no need to initialize
			;reg $4216-$4217  - multiplication or remainder results, no need to initialize

			;regs $4218-$421f  - JoyPad read registers, no need to initialize

			;regs $4300-$437F
			;no need to intialize because DMA was disabled above
			;also, we're not sure what all of the registers do, so it is better to leave them at
			;their reset state value

  JSR ClearVRAM      ;Reset VRAM
  JSR ClearPalette   ;Reset colors

  ;**** clear Sprite tables ********

  STZ $2102	;sprites initialized to be off the screen, palette 0, character 0
  STZ $2103
  LDX #$0080
  LDA #$F0
_Loop08:
  STA $2104	;set X = 240
  STA $2104	;set Y = 240
  STZ $2104	;set character = $00
  STZ $2104	;set priority=0, no flips
  DEX
  BNE _Loop08

  LDX #$0020
_Loop09:
  STZ $2104		;set size bit=0, x MSB = 0
  DEX
  BNE _Loop09

  ;**** clear WRAM ********

  STZ $2181		;set WRAM address to $000000
  STZ $2182
  STZ $2183

  LDX #$8008
  STX $4300         ;Set DMA mode to fixed source, BYTE to $2180
  LDX #wram_fill_byte
  STX $4302         ;Set source offset
  LDA #:wram_fill_byte
  STA $4304         ;Set source bank
  LDX #$0000
  STX $4305         ;Set transfer size to 64k bytes
  LDA #$01
  STA $420B         ;Initiate transfer

  LDA #$01          ;now set the next 64k bytes
  STA $420B         ;Initiate transfer

  PHK			;make sure Data Bank = Program Bank
  PLB

  CLI			;enable interrupts again

  LDX $4372  	;get our return address...
  STX $1FFD
  LDA $4374
  STA $1FFF
  RTL

wram_fill_byte:
.db $00

;----------------------------------------------------------------------------
; ClearVRAM -- Sets every byte of VRAM to zero
; In: None
; Out: None
; Modifies: flags
;----------------------------------------------------------------------------
ClearVRAM:
   pha
   phx
   php

   REP #$30		; mem/A = 8 bit, X/Y = 16 bit
   SEP #$20

   LDA #$80
   STA $2115         ;Set VRAM port to word access
   LDX #$1809
   STX $4300         ;Set DMA mode to fixed source, WORD to $2118/9
   LDX #$0000
   STX $2116         ;Set VRAM port address to $0000
   STX $0000         ;Set $00:0000 to $0000 (assumes scratchpad ram)
   STX $4302         ;Set source address to $xx:0000
   LDA #$00
   STA $4304         ;Set source bank to $00
   LDX #$FFFF
   STX $4305         ;Set transfer size to 64k-1 bytes
   LDA #$01
   STA $420B         ;Initiate transfer

   STZ $2119         ;clear the last byte of the VRAM

   plp
   plx
   pla
   RTS

;----------------------------------------------------------------------------
; ClearPalette -- Reset all palette colors to zero
; In: None
; Out: None
; Modifies: flags
;----------------------------------------------------------------------------
ClearPalette:
   PHX
   PHP
   REP #$30		; mem/A = 8 bit, X/Y = 16 bit
   SEP #$20

   STZ $2121
   LDX #$0100
ClearPaletteLoop:
   STZ $2122
   STZ $2122
   DEX
   BNE ClearPaletteLoop

   PLP
   PLX
   RTS

.ENDS
