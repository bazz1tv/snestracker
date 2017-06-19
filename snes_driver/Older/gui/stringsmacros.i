; SetCursorPos  y, x 
.MACRO SetCursorPos
	ldx #32*\1 + \2
	stx Cursor
.ENDM


.MACRO PrintString
	LDX #STRlabel\@
	JSR PrintF
	BRA END_STRlabel\@

STRlabel\@:
	.DB \1, 0
END_STRlabel\@:

.ENDM