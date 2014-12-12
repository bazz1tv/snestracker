

;=== Include MemoryMap, VectorTable, HeaderInfo ===
.INCLUDE "cpu/memorymap.i"
.include "cpu/loadgraphics.i"
.include "cpu/initsnes.i"
.include "cpu/strings.i"
.include "cpu/fadein.i"
.include "cpu/enums.i"
.include "apu/enums.i"
.include "cpu/TimerOptionsScreen.i"
.include "cpu/SNESRegs.i"
;=== Global Variables ===


;==============================================================================
; main
;==============================================================================

.BANK 0 SLOT 0
.ORG 0
.SECTION "TimerOptionsCode"

_INIT:
	php
	rep #$30
	
	
	stz TimerScreenSub1InputMode
	stz TimerNavRow
	stz TimerScreenInputMode
	rep #$20
	lda #$0106
	sta TimerCursorPos
	lda #$0202
	sta TimerValueCursorPos
	lda #$0302
	sta TimerCountValCursorPos
	
	;;;jsr InitTimerCountValBG2CopyPos
	lda TimerCountValCursorPos
	pha
	and #$ff00
	lsr
	lsr
	lsr
	; that's the equiv. of 5 shifts left from LSB;
	sta temp.w
	pla
	and #$00ff
	clc
	adc temp.w
	; word address to byte address << 1
	
	clc
	adc #12
	
	asl
	sta TimerCountValBG2CopyPos
;	;;;;;
	; jsr InitSNESTimerValueBG2CopyPos
	;;;;
	lda TimerValueCursorPos
	pha
	and #$ff00
	lsr
	lsr
	lsr
	; that's the equiv. of 5 shifts left from LSB;
	sta temp.w
	pla
	and #$00ff
	clc
	adc temp.w
	; word address to byte address << 1

	clc
	adc #12

	asl
	sta SNESTimerValueBG2CopyPos
	
	phd
	lda #TimerOptionsCursorTable_RawValues
	tcd
	
	ldx #0
	;;;;;
-	lda ($00, X)
	pha
	and #$ff00
	lsr
	lsr
	lsr
	sta temp.w
	pla
	and #$00ff
	clc
	adc temp.w
	sta ($06,X)
	
	inx
	inx
	cpx #3*2
	beq +
	bra -
+
	pld
	
	
	plp
	rts
	
	
TimerOptionsCursorTable_RawValues:
	.dw TimerCursorPos, TimerValueCursorPos, TimerCountValCursorPos
	.dw TimerCursorPos_RamLocation, TimerValueCursorPos_RamLocation, TimerCountValCursorPos_RamLocation
	
TimerOptions:	

	rep #$10
	sep #$20
	
	jsr UploadSelectionRect	; tile #1
	jsr UploadUnderline		; Tile #2
	jsr PlaceSelectionRect
	
	
	; do some color addition
	lda #%00000010
	sta $2130
	lda #%00100100
	sta $2131
	
	lda #%00000010
	sta $212D
	
	
	
	
	; Load Colors for Palette #17,18
	
	
	lda #17
	sta $2121
	lda #%01001010
	sta $2122
	lda #%01000000
	sta $2122
	lda #%10000110	; color 18
	sta $2122
	lda #%01000000
	sta $2122
	lda #%00001101		; color 19 WHItE
	sta $2122
	lda #%00010100
	sta $2122
	
	
	jsr QuickSetup	; set VRAM, the video mode, background and character pointers, 
	
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
	
	; INIT
	jsr _INIT
	
	sep #$20
	
	php
	rep #$30
	lda TimerCursorPos
	sta Underline_DestinationAddress
	ldy #5
	sty Underline_DestinationTileCount
	jsr DisplayUnderline
	plp
					
	rep #$30
	sep #$20			
	
	lda #$81
	sta $4200
	
	jsr GetSPCRamValues
	
	fadein 1, $8000
	

_forever:
	wai	

	;React to Buttons
	jsr TimerOptionsScreenJoy
	
	
	
	
	jsr DisplayStrings
	
	jsr ClearSelectionRect
	jsr PlaceSelectionRect
	
	jsr ClearUnderlineLocations
	php
	rep #$30
	lda Underline_DestinationAddress
	ldy Underline_DestinationTileCount
	jsr DisplayUnderline
	plp
	; Display our Underlines
	
	
	
	jmp _forever



GetSPCRamValues:
	php
	rep #$10
	sep #$20
	
; GET RAM VAL (Byte or Word)
; Port2 = SPC Addr
; Port1 = Command 01 (
; write ports
; wait for 01 on port 1
; load port2+3 for byte/word result
	ldx #SPCTimerSelect
	jsr SPCReadRam
	sta SNESTimerSelect

	ldx #SPCTimerValue
	jsr SPCReadRam
	sta SNESTimerValue
	
	ldx #SPCTimerCountValue
	jsr SPCReadRam
	rep #$20
	sta SNESTimerCountValue
	
	plp
	rts
; you can optimize this later
; make it a jsr routine instead of a macro, save space
.macro SetCursorPosB ARGS Pos
.index 16
	ldx Pos
	stx.w Cursor
.endm

DisplayStrings:
	php
	rep #$10
	sep #$20
	
	
	
	; not faulty
	
	SetCursorPosB TimerCursorPos_RamLocation
	PrintString "TIMER: 1 2 3"
	
	
	ldy #SNESTimerValue
	SetCursorPosB TimerValueCursorPos_RamLocation
	PrintString "Timer Val: $%x"

	ldy #SNESTimerCountValue
	SetCursorPosB TimerCountValCursorPos_RamLocation
	PrintString "Count Val: $%x"
	
	PrintString "\n\n\n\n\n\n\n\n Press A To Edit Values "
	
	plp
	rts
	
String1: 
	.dw TimerCursorPos
	.db "Timer: 1 2 3",0
String2: 
	.dw TimerValueCursorPos
	.db "Timer Val: $  ", 0
String3:
	 .dw TimerCountValCursorPos
	.db "Count Val: $    ", 0




.ENDS

