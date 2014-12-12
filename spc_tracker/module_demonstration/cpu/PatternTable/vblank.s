.include "cpu/memorymap.i"
.include "cpu/LoadGraphics.i"
;.include "cpu/PatternTable/enums.i"

.BANK 0
.orga $8000
.section "PatternTablevblank" SEMIFREE
PatternTableVBlank:
.index 16
.accu 16
	sep #$20		; mem/A = 8 bit, X/Y = 16 bit
	
    lda #$80
    sta $2115
    ldx #$c00        ; DEST
    stx $2116       ; $2116: Word address for accessing VRAM.
    lda #:bg2copy        ; SRCBANK
    ldx #bg2copy.w        ; SRCOFFSET
    ldy #$800         ; SIZE
    jsr LoadVRAM
	
	jsr transfer_bg3
	
	lda SNESbPlaySong
	;beq #PatternListMode_Edit
	beq +
	; use already used color
	jsr SetColor19Active
	bra ++
	;else
	
	
	; use this if the mode is navigation and song is not playing
+	lda #17
	sta $2121
	lda #%11000011
	sta $2122
	lda #%01110000
	sta $2122
	
	
++
	;update the joypad data
	JSR UpdatePad
	
	
	

	REP #$30		;A/Mem=16bits, X/Y=16bits
	
	inc FrameNum

	JML ReturnVBlank
.ends

.orga $8000	
.section "derp" SEMIFREE
SetColor19Active:
	.index 16
	.accu 8
	lda #17
	sta $2121
	lda #%00001101		; color 19 WHItE
	sta $2122
	lda #%00010100
	sta $2122
	rts
	
.ends
