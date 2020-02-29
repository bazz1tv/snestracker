
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
	
	;===
	; STORE SOME FAKE VALUES FOR THE TIMERS
	MOV A, #0
	MOV SPCTimerSelect, A
	MOV A, #$09
	MOV SPCTimerValue, A
	MOV A, #$d5
	MOV SPCTimerCountValue, A
	; THESE WOULD NORMALLY BE SELECTED BY THE USER IN THE GUI
	
	mov a, SPCTimerSelect
	clrc
	adc a,#COUNTERBASE
	mov SPCCounterAddr, a
	
	; SETUP TIMER
	MOV a, SPCTimerSelect
	clrc
	adc a, #$fa
	mov SPCTimerAddr, a
	mov x,SPCTimerAddr
	MOV A,SPCTimerValue
	MOV (X),A
	; THAT WILL BE THE STANDARD CONVERSION CODE FOR TIMER SETUPS
	
	; START TIMER
	MOV CONTROL, #$01	; THIS WILL DESTROY ALL OTHER TIMERS
	
	mov a,#0
	MOV SPCPatternList_CurrentEntry, a
	mov SPCSelectedPatternSequenceCounter, #0
	
	mov SPCbPlaySong, #1
	
; Enter MAIN PROTOCOL LOOP
MainLoop:
	mov a, SPCbPlaySong
	beq Protocol
		call !PlayPattern

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
    cmp a,#SPCReadRamCmd
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
    

INITPATTERNS:
	; INIT PATTERN 0 PATTERN DATA
	
	MOV Y, #0
	
	; 1
	MOV A, #$00
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A, #$08
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A,#01
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	
	; 2
	MOV A, #$80
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A, #$08
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A, #01
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	;3
	MOV A, #$00
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A, #$09
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#01
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	;4
	MOV A, #$80
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A, #$09
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A,#1
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	;5
	MOV A, #$20
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A, #$0a
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#01
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	;6
	MOV A, #$b0
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A, #$0a
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A, #1
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	
	;7
	MOV A, #$50
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A, #$0b
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#01
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y
	
	;8
	MOV A, #$00
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A, #$0c
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#01
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV A,#127
	MOV [SPCPattern0Ptr]+Y,A
	INC Y

	MOV Y,#0

	; INIT PATTERN 1
	
	; 1
	MOV Y, #0
	MOV A, #$b0
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #$0c
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A,#01
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A,#127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	
	MOV A, #$80
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #$0d
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A,#1
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y




	MOV A, #$40
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A,#$0e
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A,#1
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y



	MOV A, #$20
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #$0f
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #1
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y

	MOV A, #$00
	MOV [SPCPattern1Ptr]+Y,A
	INC Y

	MOV A, #$10
	MOV [SPCPattern1Ptr]+Y,A
	INC Y

	MOV A,#01
	MOV [SPCPattern1Ptr]+Y,A
	INC Y

	MOV A,#127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y

	MOV A,#0
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #1
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y




	MOV A, #00
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A,#1
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y



	MOV A, #00
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #1
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	MOV A, #127
	MOV [SPCPattern1Ptr]+Y,A
	INC Y
	RET
	
;-------------
; INIT
;-------------
_Init:
	MOV $F2, #$7D
	MOV $F3, #0

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
    MOV $F3, #127   ; L/R VOL 100% BOTH VOICES 
    MOV $F2, #$1C
    MOV $F3, #127
    
    MOV $F2, #$6C   ; TURN OFF MUTE
    MOV $F3, #%00100000
	
	; THIS WASN'T USED IN THE PATTERN DEMO
	; INITIALIZE THE SPCPatternList
	MOV SPCPatternListLength, #4
	MOV X, #0
	MOV A, #0
	MOV SPCPatternList+X, A
	INC X
	MOV A, #1
	MOV SPCPatternList+X, A
	INC X
	MOV A, #0
	MOV SPCPatternList+X, A
	INC X
	MOV A, #1
	MOV SPCPatternList+X, A
	; LENGTH 3, PATTERN 0, 1 ,1
	;MOV SPCPatternList_CurrentEntry, #0
	
	
	
	
	; INIT PATTERN 0 VOICE 0
	; LETS PLAY SOMETHING AT 0 AND NOTHING FOR ANYTHING ELSE
	; PATTERN FORMAT: NOTE, INSTRUMENT, VOLUME	; 4 BYTES
	; LOOK-UP TABLE FOR KEYBOARD KEYS 0-88
	
	; INIT PATTERN 0 TABLE ENTRY
	MOV A, #8
	MOV SPCPattern0Len, A
	MOV A, #00
	MOV SPCPattern0PtrLO, A
	MOV A, #$82
	MOV SPCPattern0PtrHI, A
	
	; INIT PATTERN 1 TABLE ENTRY
	MOV A, #8
	MOV SPCPattern1Len, A
	MOV A, #00
	MOV SPCPattern1PtrLO, A
	MOV A, #$83
	MOV SPCPattern1PtrHI, A
	
	CALL !INITPATTERNS
	
	; OK THE PATTERN IS READY
	; LETS ITERATE THROUGH A PATTERN
	; 
	ret

.ENDS
	


;EOF