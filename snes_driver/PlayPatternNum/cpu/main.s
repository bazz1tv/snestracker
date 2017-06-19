;============================================================================
; Includes
;============================================================================
.include "common.h"
;== Include MemoryMap, Vector Table, and HeaderInfo ==
.include "cpu/memorymap.i"

;== Include SNES Initialization routines ==
.include "cpu/initsnes.i"
.include "cpu/loadgraphics.i"
;.include "cpu/enums.i"
.include "cpu/fade.i"
.include "cpu/strings.i"

;============================================================================
; Main Code
;============================================================================

.BANK 0
.orga $8000
.SECTION "MainCode" FORCE

Start:
	InitSNES
	
	
.index 16
.accu 8
	lda #1
	sta $420d
	jml FastStart
.base $80
FastStart:
	lda #$80
	pha
	plb
	; Vram WRAM cleared
	
	; This Will Load the SPC Binary to the SPC700 (along with samples)
	ldx.w #spx_binary.w
	lda #:spx_binary	
	stx spx_binary_loc.w
	sta spx_binary_loc.w+2
	jsr LoadMusic

	; From Here, We can do as we please :) 
	

	; INIT HERE
	jsr InitUART
	jsr InitScreen
	jsr RINGFIFO_RESET
	ldx #MainVBlank
	stx VBlankPtr.w
	lda #:MainVBlank
	sta VBlankPtr.w+2
	; THEN, when READY
	
	SetCursorPos 0,0
	PrintString "MIDI Byte Diagnostic\n\n"
	
	lda #$81
	sta $4200
		
	
	cli
	fade 1, $8000,1,15


+
forever:
	WAI
	WAI
	rep #$10
	sep #$20
	
	sei
	jsr ProcessMidiPackets
	rep #$30
		lda Cursor.b
		cmp #$360
		bcc +
		sep #$20
		lda #:TextBuffer
		sta $2183
		ldx #TextBuffer+$40
		stx $2181
		;stz $2183
		
		ldx #$8008
		stx $4300
		lda #:_zero
		ldx #_zero+1
		stx $4302
		sta $4304
		ldx #$0400	; $10000
		stx $4305

		lda #%00000001
		sta $420b
		rep #$20
		lda #$0040
		sta Cursor.b
		
		cli
+
	jmp forever
	
real_time_messages:
.db $f8, $f9, $fa, $fb, $fc, $fd, $fe, $ff, $00
ProcessMidiPackets:
.index 16
.accu 8
	;sei
	nop
	nop
	nop
	nop
	;mov a,spcport2 ; MIDI Byte
	;----------------------------------------------------
	; MIDI PROCESSING
	;----------------------------------------------------
	; 
	; Check Message vs. Data (MSB set = message)
	;mov MidiByte, A
	;mov a,#SPCMidiByteCmd		; pass back cmd to SNES to tell it we're done
	;mov spcport1,a
	;mov A, MidiByte
	rep #$20
	lda RINGFIFO_WRIDX.b
	sec
	sbc RINGFIFO_RDIDX.b
	
	and #RINGFIFO_MASK
	sep #$20
	bne +
	;cli
	;nop
	;nop
	;nop
	jmp _midi_exit
+
	
	;jsr ResetCursor
	;;
	
	;;
	;jsr RINGFIFO_RD;
	;;
	rep #$30
	lda RINGFIFO_RDIDX.b
	;tax
	and #RINGFIFO_MASK.w
	inc RINGFIFO_RDIDX.b
	sta temp1
	;ldy #temp1.w
	

	tax
	sep #$20
	lda RINGFIFO_BUFFER.l,X
	sta MidiByte.b
	PrintHexNum temp2.b
	PrintString ","
	;sta temp2
	;stz temp2+1
	;ldy #temp2.w
	;PrintString "RINGFIFO_RD: "
	;PrintHexNum temp2
	;PrintString " from "
	;PrintHexNum temp1
	;PrintString "\n"
	;lda temp2
	;;
	;sta MidiByte.b
	;cli
	;nop
	;nop
	;nop
	
	;pha
	;PrintHexNum MidiByte
	;PrintString ","
	;pla
	lda MidiByte.b
	bit #$80 ;and a, #$80
	beq _data
	; Message Section
	;------------------
	; Is it real time message???
	ldx #0 ;mov X, #0
-	lda real_time_messages.w,X ;mov A, !real_time_messages+X
	beq _it_is_NOT_real_time_message
	
	inx ;inc X
	cmp MidiByte.b;cmp A, MidiByte
	bne -
	;IT IS A REAL TIME MESSAGE
	; Not Implemented YET
	jmp _midi_done ;jmp !_midi_done
_it_is_NOT_real_time_message:
	;PrintString "Not Real Time Msg\n"
	;PrintString "Storing in RunningStatusBuffer\n"
	lda MidiByte.b ;mov a, MidiByte
	sta RunningStatusBuffer.b ;mov RunningStatusBuffer, A;
	stz ThirdByteFlag.b	;mov ThirdByteFlag, #0;
	; ------Is this a tune request ?? (NOT IMPLEMENTED NOW)
	;
	;
	jmp _processing_loop_end
	;------------------
	; DATA SECTION
	;------------------
_data:
	; Third Byte Flag = 1 ???
	lda ThirdByteFlag.b	;mov A, ThirdByteFlag;
	beq _ThirdByteFlag_is_zero
	; ThirdByteFlag is 1
	; (code here)
	;
	;
	stz ThirdByteFlag.b		;	mov ThirdByteFlag, #0	
	lda MidiByte.b			;	mov A, MidiByte 		
	sta MidiData1.b			;	mov MidiData1, A		
	;PrintString "Processing a real byte!!!\n"
	jsr MidiCommandProcessor	;call !MidiCommandProcessor
	bra _processing_loop_end

_ThirdByteFlag_is_zero:
	; Running Status buffer????
	lda RunningStatusBuffer.b	;mov A, RunningStatusBuffer	;lda RunningStatusBuffer.b
	beq _processing_loop_end	; IGNORE DATA BYTE if RunningStatusBuffer == 0

	; RunningStatusBuffer != 0 ... 
	cmp #$C0	; cmp A, #$C0
	bcc _commands_less_than_c0

_commands_less_than_c0:
	;PrintString "Processing commands less than C0\n"
	inc ThirdByteFlag.b
	lda RunningStatusBuffer.b	;mov A, RunningStatusBuffer.b
	sta MidiStatus.b	;mov MidiStatus, A
	lda MidiByte.b	;mov A, MidiByte
	sta MidiData0.b	;mov MidiData0, A
	; Purposely runs into the end of loop below

_midi_done:
_processing_loop_end:


_midi_exit:
	
rts
	
MidiCommandProcessor:
.accu 8
.index 16
	
	lda MidiStatus.b	; mov A, MidiStatus	
	cmp #$90			;cmp A, #$90
	bne +
	; NOTEON PROCESSING
	;PrintString "Sending NoteON Cmd!\n"
	ldx.w MidiData0.b
	jsr SPCMidiNote;call !MidiNoteCmdProcessing
	
+
	;else... uncoded
	
_end_processing:
	stz MidiStatus.b;mov MidiStatus,#0
	stz MidiData0.b;mov MidiData0, #0
	stz MidiData1.b;mov MidiData1, #0
	;ret
	
rts
	
zero:
.db $00
ResetCursor:
	php
+	rep #$30
	lda Cursor.b
	cmp #$360
	bcc +
	lda #:TextBuffer
	sta $2181
	ldx #TextBuffer+$40
	stx $2182
	;stz $2183
	sep #$20
	ldx #$8008
	stx $4300
	lda #:_zero
	ldx #_zero+1
	stx $4302
	sta $4304
	ldx #$0400-$40	; $10000
	stx $4305

	lda #%00000001
	sta $420b
	rep #$20
	lda #$0040
+	sta Cursor.b
	plp
rts
virq:
	sei
	jml fastirq
.base $80
fastirq:
	rep #$30		;A/Mem=16bits, X/Y=16bits
		;phb
		pha
		phx
		phy
		;phd 
	;sei
	rep #$10
	sep #$20
	;jsr SPCCommand0
_byteloop:
	lda $6005	;
_check_for_fifo_overrun2:
	bit #1
	bne _uart_exit
	bit #2
	beq _noerrorsMidiByte 
		pha
		PrintString "FIFO OverRun, "
		pla
		lda $6000
		bra _uart_exit
		
		
_skip_error_checking:		
_noerrorsMidiByte:
	bit #$80
	beq +
	lda $6000
	bra _uart_exit
+
	
	
	lda $6000	; Load from FIFO -> A
	sta $6000	; TRANSMIT it for KICKS
	sta temp1.b
	;PrintHexNum temp1
	;PrintString ","
	;stz temp2+1
	
	
	;bra _byteloop
	
	
	rep #$20
	lda RINGFIFO_WRIDX.b
	and #RINGFIFO_MASK.b
	tax
	inc RINGFIFO_WRIDX.b
	;stx temp1
	sep #$20
	;pla
	
	;PrintString "RINGFIFO_WR: "
	
	;phb
	;lda #:RINGFIFO_BUFFER
	;pha
	;plb
	lda temp1.b
	sta RINGFIFO_BUFFER.l,X
	;plb
	
	;inc SuperMidiByte.b
	
	
	;bra _byteloop

_uart_exit:
	
	rep #$30
	;PLD 
	PLY 
	PLX 
	PLA 
	;PLB 
	cli
rti


;/* Buffer read / write macros                                                 */
;#define RINGFIFO_RESET(ringFifo)      {ringFifo.RINGFIFO_RDIDX = ringFifo.RINGFIFO_WRIDX = 0;}
RINGFIFO_RESET:
	php
	rep #$20
	stz.w RINGFIFO_RDIDX.b
	stz.w RINGFIFO_WRIDX.b
	plp
rts

;#define RINGFIFO_WR(ringFifo, dataIn) {ringFifo.data[RINGFIFO_MASK & ringFifo.RINGFIFO_WRIDX++] = (dataIn);}
; 8bit write of A
RINGFIFO_WR:
.index 16
.accu 8
	sta temp2
	;stz temp2+1
	ldx RINGFIFO_WRIDX.b
	rep #$20
	txa
	inx
	and #RINGFIFO_MASK.w
	stx RINGFIFO_WRIDX.b
	tax
	;stx temp1
	sep #$20
	;pla
	
	;PrintString "RINGFIFO_WR: "
	PrintHexNum temp2
	
	;PrintString "to "
	;PrintHexNum temp1
	PrintString ","
	lda temp2
	sta RINGFIFO_BUFFER.l,X

rts
;#define RINGFIFO_RD(ringFifo, dataOut){ringFifo.RINGFIFO_RDIDX++; dataOut = ringFifo.data[RINGFIFO_MASK & (ringFifo.RINGFIFO_RDIDX-1)];}
; 8 Bit Return of A
RINGFIFO_RD:
.accu 8
.index 16
	;inc RINGFIFO_RDIDX

	rep #$30
	lda RINGFIFO_RDIDX.b
	tax
	and #RINGFIFO_MASK.w
	inx
	stx RINGFIFO_RDIDX.b
	sta temp1
	;ldy #temp1.w
	

	tax
	sep #$20
	lda RINGFIFO_BUFFER.l,X
	sta temp2
	stz temp2+1
	;ldy #temp2.w
	PrintString "RINGFIFO_RD: "
	PrintHexNum temp2
	PrintString " from "
	PrintHexNum temp1
	PrintString "\n"
	lda temp2
	rts
;#define RINGFIFO_EMPTY(ringFifo)      (ringFifo.RINGFIFO_RDIDX == ringFifo.RINGFIFO_WRIDX)
; return 1 if empty
RINGFIFO_EMPTY:
.accu 8
	php
	rep #$20
	lda RINGFIFO_RDIDX.b
	sec
	sbc RINGFIFO_WRIDX.b
	bne +
	lda #1
	plp
	rts
+	lda #0

	plp
rts
;#define RINGFIFO_FULL(ringFifo)       ((RINGFIFO_MASK & ringFifo.RINGFIFO_RDIDX) == (RINGFIFO_MASK & (ringFifo.RINGFIFO_WRIDX+1)))
; return 1 if Full
RINGFIFO_FULL:
	php
	rep #$30
	lda.w RINGFIFO_RDIDX.b
	and #RINGFIFO_MASK.w
	sta temp1.w
	lda.w RINGFIFO_WRIDX.b
	ina
	and.w #RINGFIFO_MASK.w
	sec
	sbc.w temp1
	bne +
	lda #1
	plp
rts
+	lda #0	
	plp
	rts
;#define RINGFIFO_COUNT(ringFifo)      (RINGFIFO_MASK & (ringFifo.RINGFIFO_WRIDX - ringFifo.RINGFIFO_RDIDX))
; Expects ACCU=16bit
; returns Count in ACCU
RINGFIFO_COUNT:
.accu 16
	lda RINGFIFO_WRIDX.b
	sec
	sbc RINGFIFO_RDIDX.b
	and #RINGFIFO_MASK
rts
;/* buffer type                                                                */
;typedef struct{
;    uint16_t size;
;    uint16_t RINGFIFO_WRIDX;
;    uint16_t RINGFIFO_RDIDX;
;    uint8_t data[RINGFIFO_SIZE];
;} RingFifo_t;
;RingFifo_t gUartFifo;

;;;;;;; HERP DERP
	
	
	
	; RECEIVE BYTES
;----------------------------------------------------
	;jmp _skip_error_checking
	lda $6005	; LSR
	
	; ERRORS??
	
	; Check general errors
	
	; WE SKIP ERROR CHECKING FOR NOW
	bit #$80
	beq _check_for_fifo_overrun
	; Errors..
		pha
		PrintString "Error: "
		pla
		bit #%00001000
		beq +
			pha
			PrintString "Framing Error\n"
			pla
+		bit #%00010000
		beq +
		pha
		PrintString "Break Error\n"
		pla
+
		;sep #$10
		;jmp _midi_exit
	

_check_for_fifo_overrun:
	bit #2
	beq _noerrorsMidiByte2 
		pha
		PrintString "FIFO OverRun, "
		pla
_skip_error_checking:		
_noerrorsMidiByte2:
	; YES
;;;;;;;

.define NOTEON $90


PrintReceivedByte:
	phx
	phy
	php
	rep #$20
	sep #$10
	ldy FIFCount
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
	
	
+	lda MidiByte.b
	jsr PrintHex8_noload
	PrintString "\n"

	
	
++	plp
	ply
	plx
rts
	

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
-	sta TextBuffer.l,X
	inx
	inx
	cpx #$400
	bne -
+
	;plp
	
	sep #$20
	PrintString "MIDI Byte Diagnostic\n\n"
rts

MainVBlank:
.index 16
.accu 16
jml FastVBlank
.base $80
FastVBlank:
	sep #$20		; mem/A = 8 bit, X/Y = 16 bit
	
	jsr transfer_bg3
	
	
	
	
	
	
++
	;update the joypad data
	JSR UpdatePad
	
	
	

	REP #$30		;A/Mem=16bits, X/Y=16bits
	
	inc FrameNum

	JML ReturnVBlank
	
InitScreen:
	php
	rep #$10
	sep #$20
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
	
	lda #$01		;Set video mode 1, 8x8 tiles (16 PatternTableColor BG1 + BG2, 4 PatternTableColor BG3)
	sta $2105
	
	lda #$FF		
	sta $2112
	stz $2112
	
	;jsr LoadMainTrackerPatternTableColors:
	LoadPalette	BG_Palette, 0 , 4
	
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
	
	plp
rts

InitUART:
	; INIT the UART
	php
	rep #$10
	sep #$20
	
	
	
	; SET FCR = 1
	; FCR = $700002	
	lda #%00000111
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
	cpx #$10.w
	bne -
	plp
rts
	;;;;;;;


;============================================================================
; Call with Address stored in $00
LoadMusic:
.index 16
	; loop until spc is ready
	REP #$20	; 16-bit akku
scr_checkready:
	lda #$BBAA
	cmp $2140
	bne scr_checkready
 
	SEP #$20	; 8-bit akku
 
	ldy #0
 
	lda #0
	xba
 
scr_firstblock:
 
	; load spx
	lda #$01
	sta $2141
	
	lda [spx_binary_loc], y
	iny
	sta $2142
	lda [spx_binary_loc], y
	iny
	sta $2143
	
	lda #$CC
	sta $2140
 
scr_check2:
	cmp $2140
	; check for $CC
	bne scr_check2
 
 ; size bytes -> X
 	rep #$20
	pha
	lda [spx_binary_loc], y
	tax
	pla
	sep #$20
	iny
	iny
; byte to transfer
REDO:
	lda [spx_binary_loc], y
	iny
	
; store it with 0
	sta $2141
 
	lda #0
	sta $2140
	
	; check for 0
scr_check3:
	cmp $2140
	bne scr_check3
 
 ; decrement byte transfer count
	dex
 ; switch to our counter in a
	xba
 
scr_data_loop:
; load a byte
	lda [spx_binary_loc], y
	iny
	sta $2141
	xba			; counter
 	ina
	sta $2140
	
	;check port0
scr_check4:
	cmp $2140
	bne scr_check4
 
	xba			; data
 
	dex
	bne scr_data_loop
 
 	rep #$20
	pha
	lda [spx_binary_loc], y
	iny
	iny
	tax
	pla
	cpx #0
	beq scr_terminate
 	
 	sep #$20
 	; X has starting address
	lda #1	; non-zero
	sta $2141
	
	;Store it
	stx $2142
	
	
	
	; stx $2142	; address
	xba
	ina
scr_nz1:
	ina
	beq scr_nz1
 
	sta $2140
scr_check5:
	cmp $2140
	bne scr_check5
	
	and #0
	xba	; data
	
	rep #$20
	pha 
	lda [spx_binary_loc], y
	tax
	pla
	sep #$20
	iny
	iny
	
	bra REDO
 
scr_terminate:
 
	stz $2141
	rep #$20
	pha
	lda [spx_binary_loc], y
 
	sta $2142
	pla
	sep #$20
	xba
	ina
scr_nz2:
	ina
	beq scr_nz2
 
	sta $2140
scr_check6:
	cmp $2140
	bne scr_check6
 
	; PROTOCOL COMPLETE.
 
	RTS
    
.ENDS
	
.bank 0
	;.base $40
.orga $8000
.SECTION "MusicData" SEMIFREE
spx_binary:
.dw $0200, spc_end-spc_start            
		
spc_start:
        .INCBIN "apu/spc.bin"
spc_end:

.dw $0000, $200     ; 00 to finish transfer, then address to jump SPC700 too
.ENDS
.base $00




