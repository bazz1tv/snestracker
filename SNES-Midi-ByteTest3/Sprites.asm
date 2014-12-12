;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     If you use my code, please share your creations with me
;-     as I am always curious :)
;------------------------------------------------------------------------

;temporary sprite buffer1 ($0400-$061F)
.DEFINE SpriteBuf1	$0400
.DEFINE SpriteBuf2	$0600


.DEFINE sx	   		0
.DEFINE sy	   		1
.DEFINE sframe 		2
.DEFINE spriority		3

.BANK 0
.SECTION "SpriteInit"

SpriteInit:
	php	

	rep	#$30	;16bit mem/A, 16 bit X/Y

	ldx #$0000
	lda #$5555
_clr:
	sta SpriteBuf2, x		;initialize all sprites to be off the screen
	inx
	inx
	cpx #$0020
	bne _clr	

	plp
	rts
.ENDS
;==========================================================================================
