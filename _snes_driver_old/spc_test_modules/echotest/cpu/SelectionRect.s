.include "cpu/memorymap.i"
;.include "cpu/enums.i"
.include "cpu/TimerOptions/enums.i"


.bank 0
.orga $8000
.section "SelectionRect" SEMIFREE

;;;
; UPload Selection Rect
; take the Base Address as parameter in X
;
;
UploadSelectionRect:
.index 16
.accu 8
	php
	lda #$80
	sta $2115
	;ldx #$4010
	stx $2116
	rep #$20
	ldx #0
-	lda SelectionRect4bpp.w,X
	sta $2118
	inx
	inx
	cpx #$20
	bne -
	
	ldx #0
-	lda SelectionRect4bppColor2.w,X
	sta $2118
	inx
	inx
	cpx #$20
	bne -
	plp
	rts
	

; this is too hard-coded let's dynamic-afy it
;
;
;
; Y = how many times
; X = bg2copy address offset
ClearSelectionRectWithAddress:
	.index 16
	php
	rep #$20
	lda #0
-	sta bg2copy.l,X
	inx
	inx
	dey
	bne -
	plp
	rts
	
ClearSelectionRect:
	php
	rep #$30
	lda TimerLUT.w
	asl
	clc
	adc #$40
	tax
	lda #0.w
	sta bg2copy.l,X
	.rept 5
		inx
		inx
		sta bg2copy.l,X
	.endr
	
	ldx TimerCountValBG2CopyPos
	
	;lda #%0000010000000001 ;0401
	.rept 4
		sta bg2copy.l,X
		inx
		inx
	.endr
	
	
	ldx SNESTimerValueBG2CopyPos
	
	.rept 2
		sta bg2copy.l,X
		inx
		inx
	.endr
	
	
	plp
	rts
	 
PlaceSelectionRect:
	php
.index 16
	sep #$20
	jsr _placeforR0
	lda InputMode
	bne _in
	plp
	rts
_in:
	
	cmp #TimerOptionsMode_Row2
	beq _placeforR2
	cmp #TimerOptionsMode_Row1
	beq _placeforR1
	
	plp
	rts
_placeforR2:
	
	;place for LO
	rep #$20
	ldx TimerCountValBG2CopyPos
	
	lda #%0000010000000010 ;0401
	sta bg2copy.l,X
	inx
	inx
	sta bg2copy.l,X
	plp
	rts

	
_placeforR1:
	rep #$20
	ldx SNESTimerValueBG2CopyPos

	lda #%0000010000000010 ;0401
	sta bg2copy.l,X
	inx
	inx
	sta bg2copy.l,X
	plp
	rts
	
_placeforR0:
	rep #$30
	and #0
	sep #$20
	lda SNESTimerSelect
	rep #$20
	asl
	tax
	lda TimerLUT.w,X
	asl
	clc
	adc #$20*2
	tax
	sep #$20
	lda #$01
	sta bg2copy.l,X
	lda #%00000100
	sta bg2copy.l+1,X

	rts
	
TimerLUT:
	.dw 13, 15, 17
	
; Selection Rectangle
SelectionRect4bpp:
.db $ff,00,$ff,00
.db $ff,00,$ff,00
.db $ff,00,$ff,00
.db $ff,00,$ff,00
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00

SelectionRect4bppColor2:
.db $00, $ff,00,$ff
.db $00, $ff,00,$ff
.db $00, $ff,00,$ff
.db $00, $ff,00,$ff
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00

SelectionRectColr3:
.db $ff,$ff,$ff,$ff
.db $ff,$ff,$ff,$ff
.db $ff,$ff,$ff,$ff
.db $ff,$ff,$ff,$ff
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00

.ends