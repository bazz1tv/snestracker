.include "cpu/memorymap.i"
.include "cpu/LoadGraphics.i"
;.include "cpu/PatternTable/enums.i"

.BANK 0
.org 0 
.section "PatternTablevblank"
PatternTableVBlank:
.index 16
.accu 16
	sep #$20		; mem/A = 8 bit, X/Y = 16 bit
	
	lda $4210		;clear NMI Flag
	
    lda #$80
    sta $2115
    ldx #$c00        ; DEST
    stx $2116       ; $2116: Word address for accessing VRAM.
    lda #$7e        ; SRCBANK
    ldx #bg2copy         ; SRCOFFSET
    ldy #$800         ; SIZE
    jsr LoadVRAM
	
	jsr transfer_bg3
	
	

	;update the joypad data
	JSR UpdatePad
	
	
	

	REP #$30		;A/Mem=16bits, X/Y=16bits
	
	inc FrameNum
	
	

	JML ReturnVBlank
	
	
.ends
