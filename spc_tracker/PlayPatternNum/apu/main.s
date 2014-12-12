
.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/enums.i"
.INCLUDE "common.h"

;.DEFINE SPC_CODE_BLOCKSIZE SPC_CODE_END-SPCMAIN
;.EXPORT SPC_CODE_BLOCKSIZE

;.BASE $40
.BANK 0 SLOT 2
.ORGA $200
.SECTION "SPCCODE" FORCE
;SPC_TRANSFER_START:
	;.dw SPCMAIN, SPC_CODE_BLOCKSIZE

SPCMAIN:
	CLRP
	call !_Init
	
; Enter MAIN PROTOCOL LOOP
MainLoop:
	;mov a, SPCbPlaySong
	;beq Protocol
	;	call !PlayPattern

Protocol:
Poll:
    mov a,spcport1    		; Fetch io1
    cmp a,SPCPrevCmd     	; same as SPCPrevCmd?
    beq MainLoop

; New cmd
    mov SPCPrevCmd,a     	; Store as SPCPrevCmd
    cmp a,#$00      		; cmd 0?
    beq PollExit    		;
    
; cmd != 0
    ;mov y,spcport0			; io0 -> y
    ;mov SPCSnesBuffer0,y	; SNESBuffer0 <- io0
    ;mov SPCSnesBuffer1,a	; SNESBuffer1 <- io1
    ;;;
; Check cmd
	cmp a, #SPCMidiByteCmd
	bne +
	call !MidiByteRoutine
	bra PollExit
+	cmp a, #SPCMidiNoteCmd
	bne +
	call !MidiNoteCmdProcessing
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
	
KEYON:
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
_Init:
	MOV $F2, #$7D
	MOV $F3, #0			; EDL Echo Delay

	; Voice 0
    MOV $F2, #$00       ; LEFT VOLUME = 127
    MOV $F3, #127
    MOV $F2, #$01       ; RIGHT VOLUME = 127 
    MOV $F3, #127
    
    
    ; DIR
    MOV $F2, #$5D   ; STARTING ADDRESS = $9000
    MOV $F3, #$90
    
	; DIR
	; FOR PIANO
    MOV $F2, #$04       ; SOURCE NUMBER = 0 
    MOV $F3, #$00
    MOV A, #<SAMPLE1START
    MOV !$9000, A
    MOV A, #>SAMPLE1START
    MOV !$9001, A
    MOV A, #<SAMPLE1START
    MOV !$9002, A
    MOV A, #>SAMPLE1START
    MOV !$9003, A
	
	; FOR BASS
    MOV $F2, #$14       ; SOURCE NUMBER = 1
    MOV $F3, #$01
    MOV A, #<SAMPLE2START
    MOV !$9004, A
    MOV A, #>SAMPLE2START
    MOV !$9005, A
    MOV A, #<SAMPLE2START ;#$91
    MOV !$9006, A
    MOV A, #>SAMPLE2START  ; #$16
    MOV !$9007, A
    
    ; VOICE 0 PIANO
    MOV $F2, #$05 
    MOV $F3, #$00 
    MOV $F2, #$07 
    MOV $F3, #$1F ;#127   ; SET GAIN, DIRECT MODE (BIT7=0), FULL VOLUME
    ; VOICE 1 BASS
    MOV $F2, #$15 
    MOV $F3, #$00
    MOV $F2, #$17 
    MOV $F3, #$1F ;#127   ; SET GAIN, DIRECT MODE (BIT7=0), FULL VOLUME
	
    MOV $F2, #$0C 
    MOV $F3, #127   ; L/R VOL MAIN
    MOV $F2, #$1C
    MOV $F3, #127
    
    MOV $F2, #$6C   ; TURN OFF MUTE
    MOV $F3, #%00100000
	
	
	
	
	
	
	ret

.ENDS
	


;EOF