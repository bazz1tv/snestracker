;=== Include MemoryMap, VectorTable, HeaderInfo ===
.include "cpu/memorymap.i"
.include "cpu/loadgraphics.i"
.include "cpu/initsnes.i"
.include "cpu/strings.i"
.include "cpu/fade.i"
.include "cpu/PatternTable/RAM.i"
.include "cpu/SNESRegs.i"
;=== Global Variables ===

;==============================================================================
; main
;==============================================================================
.BANK 0
.orga $8000
.SECTION "PatternTable" SEMIFREE



PatternTable:
.index 16
.accu 8
	jsr _INIT
	
	stz PatternTableCol.w
	stz PatternTableRow.w
	
	lda #$81
	sta $4200
		
	cli
	fade 1, $8000,1,15


myforever:
	wai	

	;React to Buttons
	jsr PatternTableInput
	
	; Y = how many times
	; X = bg2copy address offset
	
	; Get Current Pattern Playing
	jsr GetSongPlaying
	jsr GetCurrentPattern

	jsr _ClearSelectionRects	
	jsr _ShowSelectionRect
	
	jsr _HighlightPatternVal
	
	
	jsr _DisplayStrings
	
	jmp myforever
	
_DisplayStrings:
	php
	rep #$10
	sep #$20


	; WE ARE IN PLAY MODE
	; not faulty
	
	SetCursorPos 0,0
	
	
	ldy #SNESFilterList
	PrintString "00|%x\n"
	iny
	PrintString "01|%x\n"
	iny
	PrintString "02|%x\n"
	iny
	PrintString "03|%x\n"
	iny
	PrintString "04|%x\n"
	iny
	PrintString "05|%x\n"
	iny
	PrintString "06|%x\n"
	iny
	PrintString "07|%x\n"
	iny
	PrintString "08|%x\n"
	iny
	PrintString "09|%x\n"
	
	lda SNESbPlaySong
	beq +
	PrintString "Song is Playing    "
	bra ++
+	PrintString "Song is not Playing"
++
	
	plp
	rts
	
_ClearSelectionRects:
.index 16
.accu 8
	php
	rep #$10
	sep #$20
	ldy #5
	;rep #$20
	lda #0
	sta PatternTableRow.w
	xba
	;sep #$20
-	sep #$20
	lda PatternTableRow.w
	cmp #10
	beq +
	jsr TranslatePatternTableRowtoVRAMByteAddress
	tax
	
	jsr ClearSelectionRectWithAddress
	inc PatternTableRow.w
	ldy #5
	bra -
+	plp
	stz PatternTableRow.w
	rts
	
_ShowSelectionRect:
.index 16
.accu 8
	php
	ldy #2
	and #0
	xba
	lda SNESFilterList_CurrentEntry.w
	jsr TranslatePatternTableRowtoVRAMByteAddress
	tax
	
	rep #$20
	lda #%0000010000000001
	.rept 2
		sta bg2copy.l,X
		inx
		inx
	.endr
	
	plp
	;stz PatternTableRow
	rts
	
	; Expects VRAM
TranslatePatternTableRowtoVRAMByteAddress:
.accu 8
.index 16
	php
	rep #$30
	.rept 6
		asl
	.endr
	sep #$20
	clc
	adc PatternTableCol.w
	bcc _nocarry2
	xba
	ina
	xba
_nocarry2:
	plp
	rts
	
_HighlightPatternVal:
.index 16
.accu 8
	lda InputMode
	cmp #PatternListMode_Edit
	bne _rts
	php
	;ldy #2
	and #0
	xba
	lda SNESFilterList_CurrentEntry.w
	jsr TranslatePatternTableRowtoVRAMByteAddress
	
	clc
	adc #6
	
	bcc _nocarry4
	xba
	ina
	xba
_nocarry4:
	tax

	rep #$20
	lda #%0000010000000010
	.rept 2
		sta bg2copy.l,X
		inx
		inx
	.endr

	plp
	;stz ro
_rts:
	rts

;==============================================================================
; INIT
;==============================================================================
_INIT:
.index 16
.accu 8
	lda #PatternListMode_Nav
	sta InputMode
	stz SubMode
	
	ldx #PatternTableVBlank
	stx VBlankPtr.w
	lda #:PatternTableVBlank
	sta VBlankPtr.w+2

	LoadPalette	BG_Palette, 0 , 4
	jsr LoadMainTrackerPatternTableColors
	ldx #$4010
	jsr UploadSelectionRect


	;Load Tile and Character data to VRAM
	LoadBlockToVRAM	ASCIITiles, $0000, $0800	;128 tiles * (2bit PatternTableColor = 2 planes) --> 2048 bytes

	;Set the priority bit of all the BG3 tiles
	LDA #$80
	STA $2115		;set up the VRAM so we can write just to the high byte
	LDX #$0400
	STX $2116
	;LDX #$0400		;32x32 tiles = 1024
	LDY #$2020
__next_tile:
	STY $2118
	DEX
	BNE __next_tile
        

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


	lda #%00000100		;Turn on BG1 and BG2 and BG3
      sta $212C
	  
  	; do some PatternTableColor addition
  	lda #%00000010
  	sta $2130
  	lda #%00100100
  	sta $2131
	
  	lda #%00000010
  	sta $212D

	lda #$D8		
	sta $2112
	stz $2112
	
	sta $2110
	stz $2110   
	
	lda #$01		;Set video mode 1, 8x8 tiles (16 PatternTableColor BG1 + BG2, 4 PatternTableColor BG3)
    sta $2105
	
	
	lda #17
	sta $2121
	lda #%01001010
	sta $2122
	lda #%01000000
	sta $2122
	lda #%00001101		; PatternTableColor 19 WHItE
	sta $2122
	lda #%00010100
	sta $2122
	
	
	
	; Let's load those values from SPC
	jsr LoadSPCVals
	
	rts
	
GetCurrentPattern:
.index 16
.accu 8
	php
	rep #$10
	sep #$20
	ldx #SPCPatternList_CurrentEntry
	jsr SPCReadRam
	sep #$10
	ldx SPCCommandInitiated
	bne +
	sta SNESFilterList_CurrentEntry.w

+	plp
	rts
	
GetSongPlaying:
.index 16
.accu 8
	php
	rep #$10
	sep #$20
	ldx #SPCbPlaySong
	jsr SPCReadRam
	sep #$10
	ldx SPCCommandInitiated
	bne +
	sta SNESbPlaySong

+	plp
	rts
	
LoadSPCVals:
.index 16
.accu 8
	
; GET RAM VAL (Byte or Word)
; Port2 = SPC Addr
; Port1 = Command 01 (
; write ports
; wait for 01 on port 1
; load port2+3 for byte/word result
	ldx #SPCPatternListLength
	jsr SPCReadRam
	sta SNESFilterListLength.w

	; Get SNESFilterListLength amount of entries from SPCPatternList
	ldy #0
	ldx #SPCPatternList
	
-	jsr SPCReadRam
	sta SNESFilterList,Y
	iny
	inx
	cpy SNESFilterListLength.w
	bne -
	
	rts

LoadMainTrackerPatternTableColors:	
.accu 8
.index 16
	stz $2121
	lda #%01001001
	sta $2122
	lda #%01000001
	sta $2122

	lda #1
	sta $2121
	lda #$f0
	sta $2122
	lda #$0f
	sta $2122
	rts


.ends
