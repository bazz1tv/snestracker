.include "cpu/memorymap.i"

.include "cpu/TimerOptionsScreen.i"

.bank 0
.section "TimerOptionScreenInput"

_AllTimeJoyCheck:
	lda Joy1Press
	bit #B_L
	beq +
		; Send Stop Command to SPC
		jmp SPCStopSong
		
+	bit #B_R
	beq +
		; Send Play Commdand to SPC
		jmp SPCPlaySong
		
+	bit #B_X
	beq +
	jsr GetSPCRamValues
+	rts

NavLUT:
.dw ___zero, ___one, ___two	; the underlines are AMBIGUOUS


___zero:
.index 16
.accu 8
	lda SNESTimerSelect
	sta TimerOldVal
	lda #0
	bra +
___one:
.index 16
.accu 8
	lda SNESTimerValue
	sta TimerOldVal
	lda #TimerOptionsMode_Row1
	bra +
___two:
.index 16
.accu 8
	lda SNESTimerCountValue
	sta TimerOldVal
	lda #TimerOptionsMode_Row2
+	sta TimerScreenInputMode
	rts

_Nav:
	rep #$10
	sep #$20
	
	lda Joy1Press
+	bit #B_A
	beq _nav_check_down
_derp:
	ldx.w #0
	sep #$30
	lda TimerNavRow
	asl
	tax
	rep #$10
	jmp (NavLUT,X)

	
_nav_check_down:
.accu 8
.index 16
	lda Joy1Press+1
	bit #B_DOWN
	beq _nav_check_up
	; DOWN CODE HERE
	lda TimerNavRow	; 0-2
	beq _zero
	cmp #1
	beq _one
	bcs _two
_zero:
	
	rep #$20
	lda TimerValueCursorPos
	ldy #9
	sta Underline_DestinationAddress
	sty Underline_DestinationTileCount
	inc TimerNavRow
	rts
	
_one:
	
	rep #$20
	lda TimerCountValCursorPos
	ldy #9
	sta Underline_DestinationAddress
	sty Underline_DestinationTileCount
	inc TimerNavRow
	rts
	
_two:
	
	rep #$20
	lda TimerCursorPos
	ldy #5
	sta Underline_DestinationAddress
	sty Underline_DestinationTileCount
	
	sep #$20
	stz TimerNavRow
	rts
	

	
_nav_check_up:	
+	lda Joy1Press+1
	bit #B_UP
	beq _nav_check_end
	; DOWN CODE HERE
	lda TimerNavRow	; 0-2
	beq __zero
	cmp #1
	beq __one
	bcs __two
__zero:
	
	rep #$20
	
	lda TimerCountValCursorPos
	ldy #9
	sta Underline_DestinationAddress
	sty Underline_DestinationTileCount
	sep #$20
	lda #2
	sta TimerNavRow
	rts

__one:
	
	rep #$20
	
	lda TimerCursorPos
	ldy #5
	sta Underline_DestinationAddress
	sty Underline_DestinationTileCount
	dec TimerNavRow
	rts

__two:
	
	rep #$20
	
	lda TimerValueCursorPos
	sta Underline_DestinationAddress
	ldy #9
	sty Underline_DestinationTileCount
	sep #$20
	dec TimerNavRow
	rts
	
_nav_check_end:
	rts


TimerOptionsScreenJoy:
	php
	rep #$10
	sep #$20
	
	jsr _AllTimeJoyCheck
	
	lda TimerNavRow
	bne +
	jsr TimerSelectPadCode
	jsr _Nav
	bra _mainend
+	
	lda TimerScreenInputMode
	bne +	
	; assumes nav. mode = 0
	jsr _Nav		; Process Navigation
	bra _mainend
; see what mode we are in (not in nav mode)
+	cmp #TimerOptionsMode_Row0
	bne +
	jsr TimerSelectPadCode
	bra _mainend
+	cmp #TimerOptionsMode_Row1
	bne +
	jsr _SNESTimerValueSelect
	bra _mainend
+	cmp #TimerOptionsMode_Row2
	bne _mainend
	jsr _TimerCountValueSelect
	
_mainend:
	plp
	rts

;_Row1PadCode:	
_SNESTimerValueSelect:
	rep #$10
	sep #$20
	
	lda Joy1Press
	bit #B_A
	beq +
		stz TimerScreenInputMode
+
	lda Joy1Press+1		; Up/Down
	ora Joy1Held3+1
	bit #B_B
	beq +
	pha
		lda TimerOldVal
		sta SNESTimerValue
		jsr SPCWriteTimerValue
		stz TimerScreenInputMode
	pla
+
	BIT #B_Y
	beq +
	pha
		stz SNESTimerValue
		lda SNESTimerValue
		jsr SPCWriteTimerValue
	pla
+	BIT #B_UP
	BEQ +
	pha
		INC SNESTimerValue
		lda SNESTimerValue
		jsr SPCWriteTimerValue
	pla
+	BIT #B_DOWN
	beq +
		dec SNESTimerValue
		lda SNESTimerValue
		jsr SPCWriteTimerValue
+
_rts:
	rts

	
_TimerCountValueSelect:
	rep #$30
	
	lda Joy1Press
	ora Joy1PressTemp
	sta Joy1PressTemp
	sep #$20
	inc JoyWaitTimerCountValue
	lda JoyWaitTimerCountValue
	cmp #$3
	bne _rts
	stz JoyWaitTimerCountValue 
	
	rep #$20
	lda Joy1PressTemp
	stz Joy1PressTemp
	ora Joy1Held3
	sep #$20
	xba
	BIT #B_UP
	BEQ +
	pha
		inc SNESTimerCountValue
		lda SNESTimerCountValue
		jsr SPCWriteTimerCountValueLo
	pla
+	BIT #B_DOWN
	beq +
	pha
		dec SNESTimerCountValue
		lda SNESTimerCountValue
		jsr SPCWriteTimerCountValueLo
	pla
+	
	bit #B_B
	beq +
	pha
		lda TimerOldVal
		sta SNESTimerCountValue
		jsr SPCWriteTimerCountValueLo
		stz TimerScreenInputMode
	pla
+	
	bit #B_Y
	beq +
	pha
		stz SNESTimerCountValue
		lda SNESTimerCountValue
		jsr SPCWriteTimerCountValueLo
	pla
+	xba
	bit #B_A
	beq +
	;lda SNESTimerCountValue
	;jsr SPCWriteTimerCountValueLo
	stz TimerScreenInputMode
+	rts
	

	

	


TimerSelectPadCode:
	rep #$10
	sep #$20
	
	
	lda Joy1Press
	bit #B_A
	beq _check_for_cancel
		lda SNESTimerSelect
		jsr SPCWriteTimerSelect	
		rts
_check_for_cancel:
_check_directionals:	
	lda Joy1Press+1
	bit #B_LEFT
	beq +
	
	lda SNESTimerSelect
	beq _make0equal2
	dec A
	sta SNESTimerSelect
	;lda SNESTimerSelect
	jsr SPCWriteTimerSelect	
	rts
_make0equal2:
	lda #2
	sta SNESTimerSelect
	jsr SPCWriteTimerSelect	
	rts
+	; Done checking LEFT
	lda Joy1Press+1
	bit #B_RIGHT
	beq +
	lda SNESTimerSelect
	cmp #2
	beq _make2equal0
	inc A
	sta SNESTimerSelect
	jsr SPCWriteTimerSelect	
+	rts
	
_make2equal0:
	lda #0
	sta SNESTimerSelect
	jsr SPCWriteTimerSelect	
	rts



.ends