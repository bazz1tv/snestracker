.include "cpu/memorymap.i"
.include "cpu/SNESRegs.i"
.include "apu/commands.i"
.include "cpu/enums.i"

.bank 0
.org 0
.section "SpcProtocol"

; Play Command
SPCPlaySong:
	.index 16
	.accu 8
	lda #PlaySongCmd 
	sta SNESPort1
-	lda SNESPort1
	cmp #PlaySongCmd
	bne -
	jmp SPCCommand0
	
SPCStopSong:
	.index 16
	.accu 8
	lda #StopSongCmd 
	sta SNESPort1
-	lda SNESPort1
	cmp #StopSongCmd
	bne -
	jmp SPCCommand0
	
	
SPCCommand0:
	stz SNESPort1
-	lda SNESPort1
	bne -
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
	lda #WriteRamByteCmd 
	sta SNESPort1
-	lda SNESPort1
	cmp #WriteRamByteCmd
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
	stx SNESPort2
	lda #ReadRamCmd
	sta SNESPort1
-	lda SNESPort1
	cmp #ReadRamCmd
	bne -
	rep #$20
	lda SNESPort2
	pha
	sep #$20
	jsr SPCCommand0
	rep #$20
	pla
	sep #$20
	rep #$10
	rts
	
.ends
