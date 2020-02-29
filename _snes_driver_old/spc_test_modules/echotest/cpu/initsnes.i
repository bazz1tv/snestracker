;----------------------------------------------------------------------------
; InitSNES -- my "standard" initialization of SNES memory and registers
;----------------------------------------------------------------------------
.macro InitSNES
.accu 8
.index 8
	sei			; \
	stz $4200	; Copied from SMW binary
	stz $420c	; \
	stz $420b	; \
	stz $2140	; \
	stz $2141	; \	
	stz $2142	; \
	stz $2143	; \ 
	lda #$80	; \
	sta $2100	; \
	
	clc
	xce		; 16 bit mode
	
	rep #$38
	lda #$2100
	tcd
	lda #$1fff
	tcs
	phk
	plb
	
	
	
	
	stz $2.b
	
	ldx #5.w
	ldy #0.w
	
	
-	stz $0.b,X
	inx
	inx
	cpx #$d.w
	bne -
	
	; Scroll Regs
-	stz $0.b,X
	stz $0.b,X
	inx
	inx
	cpx #$15.w
	bne -
	
	stz $16.b
	sep #$20
	
	stz $1a.b
	
	stz $1b.b
	lda #$01
	sta $1b.b
	
	sty $1c.b
	sty $1c.b
	
	stz $1e.b
	sta $1e.b
	
	sty $1f.b
	sty $1f.b
	
	ldx #$23
	
-	sty $00.b,X
	inx
	inx
	cpx #$2f
	bne -
	stz $2f.b
	lda #$30
	sta $30.b
	stz $31.b
	lda #$e0
	sta $32.b
	
	stz $33.b
	
	ldx #$4200
	phx
	pld
	
	stz $00.b
	lda #$ff
	sta $01.b
	
	sty $02.b
	sty $04.b
	sty $06.b
	sty $08.b
	sty $0a.b
	sty $0c.b

	
	
	; clear VRAM
	ldx #$4300
	phx
	pld
	
	lda #$80
	sta $2115

	ldx #$1809
_zero:
	stx $00.b
	lda #:_zero
	ldx #_zero+1
	stx $02.b
	sta $04.b
	ldx #$0000
	stx $05.b

; Clear WRAM	
	stz $2181
	stz $2182
	stz $2183

	ldx #$8008
	stx $10.b
	lda #:_zero
	ldx #_zero+1
	stx $12.b
	sta $14.b
	ldx #$0000	; $10000
	stx $15.b

	lda #%00000011
	sta $420b
	lda #2
	sta $420b
	
	ldx #0
	phx
	pld
.endm
;----------------------------------------------------------------------------
