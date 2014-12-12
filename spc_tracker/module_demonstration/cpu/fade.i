.MACRO fade ARGS Count_hi Count_lo AddVal ValTocompareFor
	
	rep #$10
	sep #$20
	ldy #Count_lo			; Count Value
	lda #AddVal		; add value
	pha				; 10,s	
	lda #ValTocompareFor		; value to compare for
	pha				; 9,s
	phy				; 7,s
	ldx #Count_hi		; 5,s
	phx
	and #0
	pha				; 4,s
	jsr fade_rt		; 2-3,s	; return address
					; 1,s	; php
	pla
	plx
	ply
	pla
	pla
.endm

