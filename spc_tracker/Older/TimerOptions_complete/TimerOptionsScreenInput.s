.include "header.i"

.include "TimerOptionsScreen.i"

.bank 0
.section "TimerOptionScreenInput"

NavLUT:
.dw ___zero, ___one, ___two	; the underlines are AMBIGUOUS


___zero:
.index 16
.accu 8
	lda TimerNum
	sta TimerOldVal
	lda #MODE_ROW0
	bra +
___one:
.index 16
.accu 8
	lda TimerRegVal
	sta TimerOldVal
	lda #MODE_ROW1
	bra +
___two:
.index 16
.accu 8
	ldx TimerCountVal
	stx TimerOldVal
	lda #MODE_ROW2
+	sta TimerScreenInputMode
	rts

_Nav:
	rep #$10
	sep #$20
	lda Joy1Press
	bit #B_A
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
	lda TimerRegValCursorPos
	ldy #9
	sta duaddr
	sty ducount
	inc TimerNavRow
	rts
	
_one:
	
	rep #$20
	lda TimerCountValCursorPos
	ldy #9
	sta duaddr
	sty ducount
	inc TimerNavRow
	rts
	
_two:
	
	rep #$20
	lda TimerCursorPos
	ldy #5
	sta duaddr
	sty ducount
	
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
	sta duaddr
	sty ducount
	sep #$20
	lda #2
	sta TimerNavRow
	rts

__one:
	
	rep #$20
	
	lda TimerCursorPos
	ldy #5
	sta duaddr
	sty ducount
	dec TimerNavRow
	rts

__two:
	
	rep #$20
	
	lda TimerRegValCursorPos
	sta duaddr
	ldy #9
	sty ducount
	sep #$20
	dec TimerNavRow
	rts
	
_nav_check_end:
	rts


TimerOptionsScreenJoy:
	php
	rep #$10
	sep #$20
	
	lda TimerScreenInputMode
	bne +	
	; assumes nav. mode = 0
	jsr _Nav		; Process Navigation
	bra _mainend
; see what mode we are in (not in nav mode)
+	cmp #MODE_ROW0
	bne +
	jsr TimerSelectPadCode
	bra _mainend
+	cmp #MODE_ROW1
	bne +
	jsr _TimerRegValSelect
	bra _mainend
+	cmp #MODE_ROW2
	bne _mainend
	jsr _TimerCountValSelect
	
_mainend:
	plp
	rts

;_Row1PadCode:	
_TimerRegValSelect:
	rep #$10
	sep #$20
	
	lda Joy1Press
	bit #B_A
	beq +
	stz TimerScreenInputMode
	bra _rts
+
	lda Joy1Press+1		; Up/Down
	ora Joy1HeldLonger+1
	bit #B_B
	beq +
	lda TimerOldVal
	sta TimerRegVal
	stz TimerScreenInputMode
	bra _rts
+
	BIT #B_UP
	BEQ +
	INC TimerRegVal
	bra _rts
+	BIT #B_DOWN
	beq +
	dec TimerRegVal
+
_rts:
	rts

	
_TimerCountValSelect:
	rep #$10
	sep #$20
	
	lda TimerCountValSelectPos
	beq _EditCountValHi
	
_EditCountValLo:
	jsr _ECVL
	bra +
_EditCountValHi
	jsr _ECVH
	
	
	
+	
	lda Joy1Press
	bit #B_A
	beq +
	;lda #SUBMODE_CHOOSECOUNTVALBYTE
	;clc
	stz TimerScreenInputMode
	rts
+
	lda Joy1Press+1
	bit #B_B
	beq +
	ldx TimerOldVal
	stx TimerCountVal
	stz TimerScreenInputMode
	rts
+
	lda Joy1Press+1
	bit #B_RIGHT
	beq +
	lda TimerCountValSelectPos
	ina
	and #1
	sta TimerCountValSelectPos
	rts
	
+	bit #B_LEFT
	beq +
	lda TimerCountValSelectPos
	dea 
	and #1
	sta TimerCountValSelectPos
+	rts
	
_ECVL:
	rep #$10
	sep #$20

	;lda Joy1Press
	;bit #B_A
	;beq +
	;stz TimerScreenInputMode
	;stz TimerScreenSub1InputMode
	;bra _rts
+
	;lda Joy1Press+1		; Up/Down
	;
	;bit #B_B
	;beq +
	;lda TimerOldVal
	;sta TimerCountVal
	;stz TimerScreenInputMode
	;stz TimerScreenSub1InputMode
	;bra _rts
+
	lda Joy1Press+1
	ora Joy1HeldLonger+1
	BIT #B_UP
	BEQ +
	inc TimerCountVal
	bra _rts
+	BIT #B_DOWN
	beq +
	dec TimerCountVal
+
	rts
	
_ECVH:
	rep #$10
	sep #$20

	
+	lda Joy1Press+1
	ora Joy1HeldLonger+1
	BIT #B_UP
	BEQ +
	inc TimerCountVal+1
	bra __rts
+	BIT #B_DOWN
	beq +
	dec TimerCountVal+1
+
__rts:
	rts

TimerSelectPadCode:
	rep #$10
	sep #$20
	
	
	lda Joy1Press
	bit #B_A
	beq _check_for_cancel
	
	; no need to update TimerNum, the changes are already in effect
	stz TimerScreenInputMode
	rts
_check_for_cancel:
	lda Joy1Press+1
	bit #B_B
	beq _check_directionals
	lda TimerOldVal
	sta TimerNum
	stz TimerScreenInputMode
	rts
_check_directionals:	
	;lda Joy1Press+1
	bit #B_LEFT
	beq +
	
	lda TimerNum
	beq _make0equal2
	dec A
	sta TimerNum
	rts
_make0equal2:
	lda #2
	sta TimerNum
	rts
+	; Done checking LEFT
	lda Joy1Press+1
	bit #B_RIGHT
	beq +
	lda TimerNum
	cmp #2
	beq _make2equal0
	inc A
	sta TimerNum
+	rts
	
_make2equal0:
	stz TimerNum
	rts



.ends