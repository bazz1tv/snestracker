; SPC upload Routine
; You need to specify a Direct-Page (Zero Page) RAM variable (24-bits)
; to store the address of your Music payloads. Call it spx_binary_loc



; Example upload
.index 16
.accu 8
	ldx.w #spx_binary
	lda #:spx_binary

	stx spx_binary_loc
	sta spx_binary_loc+2



.SECTION "MusicUploader"

LoadMusic:
.index 16
	; loop until spc is ready
scr_checkready:
	lda #$AA
	cmp $2140
	bne scr_checkready
	lda #$BB
	cmp $2141
	bne scr_checkready

	ldy #0
scr_firstblock:
	; load spx
	lda #$01
	sta $2141

	lda [spx_binary_loc], y
	iny
	sta $2142
	lda [spx_binary_loc], y
	iny
	sta $2143

	lda #$CC
	sta $2140

scr_check2:
	cmp $2140
	; check for $CC
	bne scr_check2

	; size bytes -> X
	rep #$20
	lda [spx_binary_loc], y
	tax
	sep #$20
	iny
	iny
	lda #$FF
; byte to transfer
REDO:
scr_data_loop:
; switch counter to hi-byte
	xba
; load a byte
	lda [spx_binary_loc], y
	iny
	sta $2141
	xba			; counter to lobyte
	ina
	sta $2140

	;check port0
scr_check4:
	cmp $2140
	bne scr_check4

	dex
	bne scr_data_loop

	pha
	rep #$20
	lda [spx_binary_loc], y
	beq scr_terminate
	iny
	iny
	sep #$20
	sta $2142
	xba
	sta $2143

	; X has starting address
	lda #1	; non-zero
	sta $2141
	pla
	ina
scr_nz1:
	ina
	beq scr_nz1

	sta $2140
scr_check5:
	cmp $2140
	bne scr_check5

	rep #$20
	lda [spx_binary_loc], y
	tax
	sep #$20
	iny
	iny
	lda #$FF ; reset counter too
	bra REDO

scr_terminate:
	sep #$20
	sta $2141
	rep #$20
	iny
	iny
	lda [spx_binary_loc], y
	sep #$20
	sta $2142
	xba
	sta $2143
	pla
	ina
scr_nz2:
	ina
	beq scr_nz2

	sta $2140
scr_check6:
	cmp $2140
	bne scr_check6

	; PROTOCOL COMPLETE.

	RTS

.ENDS