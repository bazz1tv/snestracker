
.include "header.i"
.include "enums.i"


; $4218
.EQU B_A		$80
.EQU B_X		$40
.EQU B_L		$20
.EQU B_R		$10
; $4219
.EQU B_B		$80
.EQU B_Y		$40
.EQU B_SELECT	$20
.EQU B_START	$10
.EQU B_UP		$08
.EQU B_DOWN		$04
.EQU B_LEFT		$02
.EQU B_RIGHT	$01

.export B_A, B_X, B_L, B_R
.export B_B,B_Y,B_SELECT, B_START, B_UP,B_DOWN, B_LEFT, B_RIGHT


.BANK 0
.SECTION "JoypadCode"


UpdatePad:
    php
    rep #$10
    sep #$20
_test1:	
	lda $4212       ; auto-read joypad status
	and #$01        ; 
	bne _test1     	; read is done when 0
	
	rep #$30        ; A/X/Y - 16 bit
	
	; Player 1
	ldx Joy1Raw		; load log of last frame's RAW read of $4218
	ldy Joy1Held
	                ; the log will be 0 the first time read of course..
	lda $4218       ; Read current frame's RAW joypad data
	sta Joy1Raw     ; save it for next frame.. (last frame log is still in X)
	txa             ; transfer last frame input from X -&gt; A (it's still in X too)
	eor Joy1Raw     ; Xor last frame input with current frame input
	                ; shows the changes in input
	                ; buttons just pressed or just released become set.
	                ; Held or unactive buttons are 0
	and Joy1Raw     ; AND changes to current frame's input.
	                ; this ends up leaving you with the only the buttons that are pressed..
	                ; It's MAGIC!
	sta Joy1Press	; Store just pressed buttons
	txa             ; Transfer last frame input from X -&gt; A again
    and Joy1Raw		; Find buttons that are still pressed (held)
    sta Joy1Held    ; by storing only buttons that are pressed both frames
    tya
    and Joy1Held	; A = presently held keys
    beq ++			; go ++ if there's no held keys
					
	
    inc Joy1HeldCount
    lda Joy1HeldCount
    cmp #$04
    bne +
    lda Joy1Held
    sta Joy1HeldLong
	bra +++
+   cmp #$0D
    bne +++
    lda Joy1Held
    sta Joy1HeldLonger
    bra +++
++	stz Joy1HeldCount
	stz Joy1HeldLong
	stz Joy1HeldLonger
+++
    ; Player 2      ; Repeat :)
    ldx Joy2Raw
	lda $421A       ; Read Joypad2 Regs
	sta Joy2Raw
	txa
	eor Joy2Raw     ; Find  just triggered buttons
	and Joy2Raw
	sta Joy2Press
	txa
    and Joy2Raw     ; Find buttons that are still pressed (held)
    sta Joy2Held
    
    ; Joypads standard (ie not a mouse or superscope..) and connected?
    sep #$20
    ldx #$0000          ; we'll clear recorded input if pad is invalid
    
    lda $4016           ; Pad 1 - now we read this (after we stored a 0 to it earlier)
    bne _check2         ; $4016 returns 0 if not connected, 1 if connected - branch if not 0
    stx Joy1Raw         ; otherwise clear all recorded input.. it's not valid..
    stx Joy1Press
    stx Joy1Held

_check2:    
    ;lda $4017           ; Pad 2
    ;bne _done           ; 0=not connected,
    ;stx Joy2Raw
    ;stx Joy2Press
    ;stx Joy2Held

_done:
    plp
    RTS
    
.ENDS
