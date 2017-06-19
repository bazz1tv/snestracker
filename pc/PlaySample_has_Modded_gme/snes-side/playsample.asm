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
merp:   
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
	bne scr_check2
 
	ldx.w spx_binary, y
	iny
	iny
	lda spx_binary, y
	iny
	sta $2141
 
	lda #0
	sta $2140
scr_check3:
	cmp $2140
	bne scr_check3
 
 
	dex
 
	xba
 
scr_data_loop:
	lda spx_binary, y
	iny
	;sta REG_APUI01
	xba			; counter
 

 
	ina
	REP #$20
	sta $2140
	SEP #$20
 
scr_check4:
	cmp $2140
	bne scr_check4

	xba			; data
 
	dex
	bne scr_data_loop
derp:
 	; entry point
 
	ldx.w spx_binary, y
	iny
	iny
	cpx #0
	beq scr_terminate
 	
 	lda #1
	sta $2141

 	stx $2142
	ldx.w spx_binary, y
	iny
	iny
	
	
	xba
	ina
scr_nz1:
	ina
	beq scr_nz1
 
	sta $2140
scr_check5:
	cmp $2140
	bne scr_check5
	lda #$ff
	xba
	bra scr_data_loop
 
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

; To programmatically generate the stuff below, ask the user to supply
; the following: 
; sample file, sample load address (in SPC ram)
; spc file, spc load address
; program will find file sizes, check for conflict, and output
; correct WLA format directives

spx_binary:
    .dw $200, 1530 ; 1080;7065; 1530
    .INCBIN "../samples/blip.brr"
    .dw $1000, 133
    .INCBIN "spc/SPC.OBJ"
    .dw $0000, $1000


;spx_binary2: 
;	.dw Sample_Start_Addr, Sample_Sizeb
;	.INCBIN "sample.brr"
;	; check here for Zero word
;	.dw Prog_Addr, Prog_Sizeb
;	.INCBIN "prog.spc"
;	.dw $0000 ; , Prog_Addr can be eliminated by using above Prog_Addr
.ENDS