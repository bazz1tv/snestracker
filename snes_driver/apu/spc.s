.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/enums.i"

; Zero page variables!
.RAMSECTION "gp-dp0" BANK 0 SLOT SPC_DP0_SLOT
temp				    dw

PrevCmd				  db
SnesBuffer0			db
SnesBuffer1			db

bFlags  			  db
.ends



.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "SPCCODE" FORCE
	
MAIN:
	CLRP ; set dp to 0
  ; We are not dealing with echo yet. so just set it so ff00 at the 
  mov dspaddr, #esa
  mov dspval, #$ff
  MOV dspaddr, #edl
  MOV dspval, #0
  
  ; TODO - Could alter master volume by song setting here
  MOV dspaddr, #mvol_l 
  MOV dspval, #127   ; L/R VOL 100% BOTH VOICES 
  MOV dspaddr, #mvol_r
  MOV dspval, #127

  ; Todo - enable echo buffer writes when bringing up echo support
  MOV dspaddr, #flg   ; TURN OFF MUTE
  MOV dspval, #%00100000 ; disable echo buffer writes
	
	;===
	; Load the timer values from their stored settings.

	; START TIMER
	MOV control, #$01	; THIS WILL DESTROY ALL OTHER TIMERS
	
; Enter MAIN LOOP
MainLoop:
	bbc bFlags.bPlaySong, Poll
  ;call !PlayPattern

Poll:
  mov a,spcport1  		; Fetch io1
  beq PollExit    		; was NULL CMD
  cmp a,PrevCmd     	; same as PrevCmd?
  beq MainLoop

; New cmd
  mov PrevCmd,a     	; Store as PrevCmd
    
; Check cmd
  cmp a, #CmdEnd
  bpl PollExit
  ; push the PollExit address to stack and do a table jump
  asl a ; word-sized index into jump table
  mov x, a
  mov a, #>PollExit
  push a
  mov a, #<PollExit
  push a
  jmp [!CmdJumpTable-2+X]
PollExit:
	mov a,PrevCmd		; pass back cmd to SNES to tell it we're done
	mov spcport1,a	; io1
  bra MainLoop   ; repoll snes

CmdJumpTable:
  .dw FetchRamValue, WriteRamByte, PlaySong, StopSong
	
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

.ENDS
;EOF
