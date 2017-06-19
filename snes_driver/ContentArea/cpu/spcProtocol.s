.include "cpu/memorymap.i"
.include "cpu/SNESRegs.i"
.include "apu/enums.i"
;.include "cpu/enums.i"

.bank 0
.org 0
.section "SpcProtocol"

; Play Command
SPCPlaySong:
	.index 16
	.accu 8
	lda #0
	ldx #SPCSelectedPatternSequenceCounter.w
	jsr SPCWriteRamByte
	lda #SPCPlaySongCmd 
	sta SNESPort1
-	lda SNESPort1
	cmp #SPCPlaySongCmd
	bne -
	jmp SPCCommand0
	
SPCStopSong:
	.index 16
	.accu 8
	lda #SPCStopSongCmd 
	sta SNESPort1
-	lda SNESPort1
	cmp #SPCStopSongCmd
	bne -
	jmp SPCCommand0
	
	
SPCCommand0:
	stz SNESPort1
-	lda SNESPort1
	bne -
	stz SPCCommandInitiated
	rts
	

;IN:
; 
; X = Addr 16
; A = writeval8
SPCWriteRamByte:
.index 16
.accu 8
	stx SNESPort2
	sta SNESPort0
	lda #SPCWriteRamByteCmd 
	sta SNESPort1
-	lda SNESPort1
	cmp #SPCWriteRamByteCmd
	bne -
	jmp SPCCommand0

; In:
; X = Addr16
; Out:
; A = val8/16
; 16bits will be filled. upper 8 may be trash depending on your goal
SPCReadRam:
	.index 16
	.accu 8
	php
	stx SNESPort2
	lda #SPCReadRamCmd
	sta SNESPort1
-	lda SNESPort1
	cmp #SPCReadRamCmd
	bne -
	rep #$20
	lda SNESPort2
	pha
	sep #$20
	jsr SPCCommand0
	rep #$20
	pla
	plp
	rts
	
SPCReadRamDuringGameLoop:
	.index 16
	.accu 8
	lda SPCCommandInitiated
	bne +
	stx SNESPort2
	lda #SPCReadRamCmd
	sta SNESPort1
	sta SPCCommandInitiated
+	lda SNESPort1
	cmp #SPCReadRamCmd
	bne +
	rep #$20
	lda SNESPort2
	pha
	sep #$20
	jsr SPCCommand0
	rep #$20
	pla
	sep #$20
	rep #$10
+	rts
	
; IN:
; A = 0-2
;
.accu 	8
.index 16
SPCWriteTimerSelect:
	ldx #SPCTimerSelect
	;jsr SPCWriteRamByte
	;
	stx SNESPort2
	sta SNESPort0
	lda #SPCSelectTimerCmd
	sta SNESPort1
-	lda SNESPort1
	cmp #SPCSelectTimerCmd
	bne -
	jmp SPCCommand0	; so you can send the same command twice
	
.accu 	8
.index 16	
SPCWriteTimerValue:
	ldx #SPCTimerValue
	;jsr SPCWriteRamByte
	stx SNESPort2
	sta SNESPort0
	lda #SPCWriteTimerValueCmd
	sta SNESPort1
-	lda SNESPort1
	cmp #SPCWriteTimerValueCmd
	bne -
	jmp SPCCommand0	; so you can send the same command twice
	rts
	
SPCWriteTimerCountValueLo:
	ldx #SPCTimerCountValue
	jsr SPCWriteRamByte
	rts
SPCWriteTimerCountValueHi:
	ldx #SPCTimerCountValue+1
	jsr SPCWriteRamByte
	rts


.ends