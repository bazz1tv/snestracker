;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     If you use my code, please share your creations with me
;-     as I am always curious :)
;------------------------------------------------------------------------
.include "header.i"
.include "LoadGraphicsMacros.i"      
.BANK 0 SLOT 0
.ORG 0
.SECTION "QuickCodeSection" SEMIFREE

.DEFINE FrameNum $12	
.define bg3_sc_location $14



VBlank:
	rep #$30		;A/Mem=16bits, X/Y=16bits
	phb
	pha
	phx
	phy
	phd

	sep #$20		; mem/A = 8 bit, X/Y = 16 bit

	jsr transfer_bg3

	;update the joypad data
	JSR GetInput

	lda $4210		;clear NMI Flag

	REP #$30		;A/Mem=16bits, X/Y=16bits
	
	inc FrameNum
	
	; Expects 16bit X/Y
	jsr reset_pixel
	jsr set_pixel
	
	rep #$30
	
	PLD 
	PLY 
	PLX 
	PLA 
	PLB 
      RTI

;============================================================================
; transfer_bg3
; DMA BG3 tilemap copy from TextBuffer
; To bg3_sc_location
transfer_bg3:
	php
	rep #$30
	pha
	phx
	phy
	rep #$10
	sep #$20
.ACCU 8
.INDEX 16
	;*********transfer BG3 data
	LDA #$00
	STA $2115		;set up VRAM write to write only the lower byte

	;LDX #$0400
	LDX bg3_sc_location
	STX $2116		;set VRAM address to BG3 tile map

	LDY #$1800
	STY $4300		; CPU -> PPU, auto increment, write 1 reg, $2118 (Lowbyte of VRAM write)
	ldy #TextBuffer16.w
	STY $4302		; source offset
	LDY #$0400
	STY $4305		; number of bytes to transfer
	LDA #TextBufferBank.b
	STA $4304		; bank address = $7F  (work RAM)
	LDA #$01
	STA $420B		;start DMA transfer
	
	rep #$30
	ply
	plx
	pla
	plp
	rts



	
	
QuickSetup:
	php

	rep #$10		;A/mem = 8bit, X/Y=16bit
	sep #$20

	;Load palette to make our pictures look correct
	LoadPalette	BG_Palette, 0 , 255

	;Load Tile and Character data to VRAM
	LoadBlockToVRAM	ASCIITiles, $0000, $0800	;128 tiles * (2bit color = 2 planes) --> 2048 bytes

	;Set the priority bit of all the BG3 tiles
	LDA #$80
	STA $2115		;set up the VRAM so we can write just to the high byte
	LDX #$0400
	STX $2116
	LDX #$0400		;32x32 tiles = 1024
	LDA #$20
__next_tile:
	STA $2119
	DEX
	BNE __next_tile

    
	lda #$01		;Set video mode 1, 8x8 tiles (16 color BG1 + BG2, 4 color BG3)
      sta $2105         

	lda #$08		;Set BG1's Tile Map VRAM offset to $0800 (word address)
      sta $2107		;   and the Tile Map size to 32 tiles x 32 tiles

	lda #$0C		;Set BG2's Tile Map VRAM offset to $0C00 (word address)
      sta $2108		;   and the Tile Map size to 32 tiles x 32 tiles

	lda #$04		;Set BG3's Tile Map VRAM offset to $0400 (word address)
      sta $2109		;   and the Tile Map size to 32 tiles x 32 tiles
	ldx #$400
	stx bg3_sc_location

	lda #$41		;Set BG1's Character VRAM offset to $1000 (word address)
      sta $210B		;Set BG2's Character VRAM offset to $4000 (word address)

	lda #$00		;Set BG3's Character VRAM offset to $0000 (word address)
      sta $210C		;


	lda #%00000101		;Turn on BG1 and BG2 and BG3
      sta $212C

	lda #$FF		;Scroll BG3 down 1 pixel
	sta $2112
	sta $2112
	
	sta $210E
	sta $210E   
	
	stz $2121
	lda #$FF
	sta $2122
	lda #%01111111
	sta $2122      

      lda #$0F		;Turn on screen, full brightness
      sta $2100		

	plp
	rts

.ENDS

;==========================================================================================

.BANK 1 SLOT 0
.ORG 0
.SECTION "CharacterData"

.incdir "gfx/ascii"
BG_Palette:
	.INCBIN "ascii.clr"
	.REPT 128
	.DW 0
	.ENDR

ASCIITiles:
	.INCBIN "ascii.pic"

.ENDS

;==========================================================================================
