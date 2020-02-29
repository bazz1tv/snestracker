
.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/enums.i"
.INCLUDE "common.h"

;.DEFINE SPC_CODE_BLOCKSIZE SPC_CODE_END-SPCMAIN
;.EXPORT SPC_CODE_BLOCKSIZE

;.BASE $40
.BANK 0 SLOT 2
; Slot 0, 1 for Direct Page
.ORGA $200
.SECTION "SPCCODE" FORCE

SPCMAIN:
	CLRP
	call !_INIT
	
	; Init Timer to increment 5 Seconds
	mov $00fa, #$80 ; 512 MS
	; run timer
	
	
	; READ EDL DELAY that much
	mov $f2, #$6d
	mov a, $f3
	mov $0, a
	; wait MAX 240 msec
	; Start counting
	mov $00f1, #1
	
	mov a,#0
	clrc
-	adc a,!$fd
	cmp a, $00
	bcc -
	
	mov $f1, #0
	
	mov $fa, #0
	
	
	; voice 0 Pitch
	mov $f2, #2
	mov $f3, #0
	mov $f2, #3
	mov $f3, #$10
	
	; SET ECHO REGION LOCATION
	; $1100
	;
	mov a, #$31
	mov $f2, #$6d
	mov $f3, a
	
	; set echo volume
	mov $f2, #$2c
	mov $f3, #127
	mov $f2, #$3c
	mov $f3, #127
	; echo ON voice 1
	mov $f2, #$4d
	mov $f3, #1
	
	;c0-c8
	mov $f2, #$0f
	mov $f3, #$ff
	mov $f2, #$1f
	mov $f3, #$08
	mov $f2, #$2f
	mov $f3, #$17
	mov $f2, #$3f
	mov $f3, #$24
	mov $f2, #$4f
	mov $f3, #$24
	mov $f2, #$5f
	mov $f3, #$17
	mov $f2, #$6f
	mov $f3, #$08
	mov $f2, #$7f
	mov $f3, #$ff
	
	mov $f2, #$7d
	mov $f3, #15
	
	mov $f2, #$0d
	mov $f3, #100
	
	;mov SPCbPlaySong, #0

	mov $f1, #1	; Start Timer 0
	
MainLoop:
	;mov a, SPCbPlaySong
	;beq Protocol
		;call !PlayPattern

Protocol:
Poll:
    mov a,spcport1    		; Fetch io1
    cmp a,SPCPrevCmd     	; same as SPCPrevCmd?
    beq MainLoop

; New cmd
    mov SPCPrevCmd,a     	; Store as SPCPrevCmd
    cmp a,#$00      		; cmd 0?
    beq PollExit    		;
    
; Check cmd
	cmp a,#SPCKonCmd
	bne +
	call !KeyonCmd
	bra PollExit
+   cmp a,#SPCReadRamCmd
    bne +
	call !FetchRamValue
	bra PollExit
+	cmp a,#SPCWriteRamByteCmd
	bne +
	call !WriteRamByte
	bra PollExit
+	cmp a,#SPCPlaySongCmd
	bne +
	call !PlaySong
	bra PollExit
+	cmp a,#SPCStopSongCmd
	bne +
	call !StopSong
	bra PollExit
+	cmp a,#SPCSelectTimerCmd
	bne +
	call !SelectTimer
	bra PollExit
+	cmp a, #SPCWriteTimerValueCmd
	bne +
	call !WriteTimerValue
	bra PollExit
+

    
PollExit:
	mov a,SPCPrevCmd		; pass back cmd to SNES to tell it we're done
	mov spcport1,a			; io1
    jmp !MainLoop        		; repoll snes
	

    
; ====== WRITE A BYTE TO THE S-SDSP =========
WDSP:
    MOV !$00F2,Y
    MOV !$00F3,A
    RET
; ====== WRITE A BYTE TO THE S-SDSP =========
WDSPX:
    MOV !$00F2,X
    MOV !$00F3,A
    RET

KeyonCmd:
	mov a, spcport0
keyon:
	; KEY ON
	MOV $F2, #$4C
	MOV $F3, A
	RET
	
KEYOFF:
	MOV $F2, #$5C
	MOV $F3, A
	RET
    
	
;-------------
; INIT
;-------------
_INIT:
	MOV $F2, #$7D		; Echo Delay
	MOV $F3, #0

    MOV $F2, #$00       ; LEFT VOLUME = 127
    MOV $F3, #127
    MOV $F2, #$01       ; RIGHT VOLUME = 127 
    MOV $F3, #127
    
    
    ; DIR
    MOV $F2, #$5D   ; STARTING ADDRESS = $9000
    MOV $F3, #$30	; $90 * $100 = $9000
    
	; DIR
	; FOR PIANO
    MOV $F2, #$04       ; SOURCE NUMBER = 0 
    MOV $F3, #$00
    MOV A, #<SAMPLE1START
    MOV !$3000, A
    MOV A, #>SAMPLE1START
    MOV !$3001, A
    ;MOV A, #<SAMPLE1START
    ;MOV !$9002, A
    ;MOV A, #>SAMPLE1START
    ;MOV !$9003, A
	
	; FOR BASS
    ;MOV $F2, #$14       ; SOURCE NUMBER = 1
    ;MOV $F3, #$01
    ;MOV A, #<SAMPLE2START
    ;MOV !$9004, A
    ;MOV A, #>SAMPLE2START
    ;MOV !$9005, A
    ;MOV A, #<SAMPLE2START ;#$91
    ;MOV !$9006, A
    ;MOV A, #>SAMPLE2START  ; #$16
    ;MOV !$9007, A
    
    ; VOICE 0 PIANO
    MOV $F2, #$05 
    MOV $F3, #$00 	; ADSR = 0
	; What about $06???
	
    MOV $F2, #$07 
    MOV $F3, #$1F ;#127   ; SET GAIN, DIRECT MODE (BIT7=0), FULL VOLUME
    
	
    MOV $F2, #$0C 
    MOV $F3, #127   ; L/R VOL 100% BOTH VOICES 
    MOV $F2, #$1C
    MOV $F3, #127
    ; Make DSP Register EQU file
    MOV $F2, #$6C   ; TURN OFF MUTE
    MOV $F3, #%00000000
	
	
	ret

.ENDS
	


;EOF