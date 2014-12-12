;== Include MemoryMap, Vector Table, and HeaderInfo ==
.INCLUDE "header.inc"

;== Include SNES Initialization routines ==
.INCLUDE "InitSNES.asm"


;============================================================================
; Main Code
;============================================================================

.BANK 0 SLOT 0
.ORG 0
.SECTION "MainCode"

Start:
    InitSNES
    
    rep #$10
    sep #$20
    
    
    
    ; set backdrop to white, know that progress is made
    
    jsr LoadMusic
    
    stz $2121
    lda #$FF
    sta $2122
    lda #$7F
    sta $2122
    
    lda #$0F
    sta $2100
    
Loop:
    jmp Loop

LoadMusic:
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
	lda #$CC
	ldx.w spx_binary, y
	iny
	iny
	stx $2142
	sta $2140
 
scr_check2:
	cmp $2140
	; check for $CC
	bne scr_check2
 
 ; size bytes -> X
	ldx.w spx_binary, y
	iny
	iny
; byte to transfer
REDO:
	lda spx_binary, y
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
	lda spx_binary, y
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
 
 
	ldx.w spx_binary, y
	iny
	iny
	cpx #0
	beq scr_terminate
 
 
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
	
	ldx.w spx_binary, y
	iny
	iny
	
	bra REDO
 
scr_terminate:
 
	stz $2141
	ldx.w spx_binary, y
 
	stx $2142
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

.SECTION "MusicData"

spx_binary: 
    .dw $0200, $2ab ;$41d+$71 ; Start Address, size_bytes(sample + Spc code)
    .INCBIN "spc.obj"
	.dw $1000, $27f0
	.INCBIN "piano.brr"
	.dw $4000, $1506
	.INCBIN "bass.brr"
    .dw $0000, $200	; 00 to finish transfer, then address to jump SPC700 too to begin code execution
.ENDS
