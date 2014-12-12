.include "cpu/memorymap.i"
.bank 0
.org 0
.section "fadein"

fadein_rt:
	php
	rep #$10
	sep #$20
	
	;FadeIn 3
	
_loop_back:
	dey
	bne _loop_back
	dey
	dex
	cpx #0
	bne _loop_back
	
	; restore initial loop values for loopback
	rep #$20
	lda 5,s
	tax
	lda 7,s
	tay
	sep #$20
	; update brightness settings
	; $01,$02 control whether the value is added ($01 = 1, $02 = $0f) or subtracted ($01 = $FF, $02 = 0)
	lda 4,s
	clc
	adc 10,s
	sta 4,s
	sta $2100
	cmp 9,s
	bne _loop_back
	plp
	rts
	
.ends