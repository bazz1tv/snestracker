; SetCursorPos  y, x 
.MACRO SetCursorPos
	ldx #32*\1 + \2
	stx Cursor
.ENDM


.MACRO PrintString
	LDX #_STRlabel\@
	JSR PrintF
	BRA _END_STRlabel\@

_STRlabel\@:
	.DB \1, 0
_END_STRlabel\@:

.ENDM