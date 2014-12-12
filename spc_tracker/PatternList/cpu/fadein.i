.MACRO fadein
	rep #$10
	sep #$20
	stz $2100
	ldy #\2
	lda #$01		; add value
	pha				; 10,s	
	lda #$0f		; value to compare for
	pha				; 9,s
	phy				; 7,s
	ldx #\1			; 5,s
	phx
	and #0
	pha				; 4,s
	jsr fadein_rt	; 2-3,s	; return address
					; 1,s	; php
	pla
	plx
	ply
	pla
	pla
.endm