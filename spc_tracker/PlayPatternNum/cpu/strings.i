; SetCursorPos  y, x 
.MACRO SetCursorPos
	ldx #32*\1 + \2
	stx Cursor
.ENDM


.MACRO PrintString
	pha
	phx
	phy
	php
	rep #$10
	LDX #_STRlabel\@
	JSR PrintF
	plp
	ply
	plx
	pla
	BRA _END_STRlabel\@

_STRlabel\@:
	.DB \1, 0
_END_STRlabel\@:

.ENDM

;here's a macro for printing a number (a byte)
;
; ex:  PrintNum $2103 	;print value of reg $2103
;      PrintNum #9	;print 9
.MACRO PrintNum
	lda \1
	jsr PrintInt8_noload
.ENDM

.MACRO PrintHexNum
	pha
	lda \1
	jsr PrintHex8_noload
	pla
.ENDM