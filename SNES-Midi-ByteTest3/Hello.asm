;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     There are many times that one just needs to write a quick
;-     program to test this or that.  This is the perfect skeleton
;-     code for that.
;- 
;-     For an example, this code prints "Hello world!!" on the screen,
;-     shows some print functions, and tests the emulator's use of the
;-     decimal flag.
;-
;-     If you use my code, please share your creations with me
;-     as I am always curious :)
;------------------------------------------------------------------------

;=== Include MemoryMap, VectorTable, HeaderInfo ===
.INCLUDE "Hello.inc"

;=== Include Library Routines & Macros ===
.INCLUDE "InitSNES.asm"
.INCLUDE "LoadGraphics.asm"
.INCLUDE "2Input.asm"
.INCLUDE "Strings.asm"
.INCLUDE "QuickSetup.asm"

;=== Global Variables ===

.ENUM $80
	var1	db
	LSR		db
	ISR		db
	temp1	dw
	temp2	dw
	temp3	dw
	FIFO		dsb 16
	FIFCount	db
	FIFPos		db
	TabCounter	db
.ENDE

;==============================================================================
; main
;==============================================================================

.BANK 0 SLOT 0
.ORGA $8000
.SECTION "MainCode" SEMIFREE

Main:
	InitializeSNES
Derp:
	; INIT the UART
	rep #$10
	sep #$20
	
	
	; SET FCR = 1
	; FCR = $700002	
	lda #%10001111
	sta $6002
	; SET LCR to 0b10000011 #$83
	; LCR = $700003
	lda #$83
	sta $6003
	lda #$20
	sta $6000
	lda #0
	sta $6001
	
	lda #$03
	sta $6003
	
	; Set IER = 0
	; cause this feeds the INT line
	; IER = $700001
	lda #0
	sta $6001
	
	stz FIFCount
	ldx #0
-	stz FIFO,X
	inx
	inx
	cpx #$40.w
	bne -
	
	stz TabCounter
	
	; that cleared the FIFO RAM space
	
	jsr QuickSetup	; set VRAM, the video mode, background and character pointers, 
				; and turn on the screen

	jsr JoyInit		;setup joypads and enable NMI
	
	SetCursorPos 0,0
	PrintString "MIDI Byte Diagnostic\n\n"
	
	sep #$30
	rep #$10
forever:
.index 16
.accu 8
	;.rept 60
	WAI
;.endr
	
	
+	
	sep #$10
	sei
	ldy FIFCount
	beq ++
	
	ldx #0
	stx temp1
	
	
	rep #$20
    LDA Cursor			;Get Current position
-	CLC
	ADC #$0020			;move to the next line
	dey
	bne -
	rep #$10
	and #$ffe0
	cmp #$360
	bcc +
	jsr ResetText
+	sep #$20
	
	
-	
	;PrintString "Midi Byte: "
	
	
+	ldx temp1
	lda FIFO,X
	jsr PrintHex8_noload
	PrintString "\n"
	;jsr ResetText
    
	inc temp1
	dec FIFCount
	bne - 
	
	
++	cli
	jmp forever

ResetText:
	;php
	
	rep #$30
	;ldx Cursor
	;bne + 
	stz Cursor
	; Clear Text Buffer $400 bytes
	;rep #$30
	
	ldx #0.w
	lda #0.w
-	sta TextBuffer,X
	inx
	inx
	cpx #$400
	bne -
+
	;plp
	
	sep #$20
	PrintString "MIDI Byte Diagnostic\n\n"
rts
	; TIME TO WRITE midi processor;
	; First we need to properly load FIFO Data
	
	; 16-byte FIFO

virq:
	;sei
	php
	sep #$30

	; Load ISR If you need to use it :)
	;lda $700002
	;sta ISR
_restart:
	; Good to Read LSR (see if FIFO needs reading, or check for errors)
	lda $6005	; LSR
	sta LSR
	
	bit #$80
	beq +
	bit #1
	beq _done
	lda $6000
	bra _done
+
	bit #1
	beq _done
	
	ldx FIFCount
	lda $6000
	sta $6000
	sta FIFO,X
	inx
	stx FIFCount
	
	
	
	
	bra _restart
	;PrintString " ISR: "
	;PrintHexNum ISR
	
_done:
	plp
	;cli
    RTI
;============================================================================
VBlank:
	rep #$30		;A/Mem=16bits, X/Y=16bits
	phb
	pha
	phx
	phy
	phd

	sep #$20		; mem/A = 8 bit, X/Y = 16 bit

	;SetCursorPos 4,1
	
	
	
	;*********transfer BG3 data
	LDA #$00
	STA $2115		;set up VRAM write to write only the lower byte

	LDX #$0400
	STX $2116		;set VRAM address to BG3 tile map

	LDY #$1800
	STY $4300		; CPU -> PPU, auto increment, write 1 reg, $2118 (Lowbyte of VRAM write)
	LDY #$0000
	STY $4302		; source offset
	LDY #$0400
	STY $4305		; number of bytes to transfer
	LDA #$7F
	STA $4304		; bank address = $7F  (work RAM)
	LDA #$01
	STA $420B		;start DMA transfer

	;update the joypad data
	JSR GetInput

	lda $4210		;clear NMI Flag

	REP #$30		;A/Mem=16bits, X/Y=16bits
	
	;inc FrameNum

	PLD 
	PLY 
	PLX 
	PLA 
	PLB 
      RTI
	


	
	
	
	




.ENDS

