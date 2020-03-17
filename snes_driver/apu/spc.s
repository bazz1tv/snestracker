.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/regdefs.i"
.INCLUDE "apu/enums.i"
.INCLUDE "apu/commands.i"

; Zero page variables!
.RAMSECTION "gp-dp0" BANK 0 SLOT SPC_DP0_SLOT
l               db
h               db

e               db
d               db

PrevCmd         db
SnesBuffer0     db
SnesBuffer1     db

flags           db
.ends

.equ hl l EXPORT
.equ de e EXPORT

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "SPCCODE" FORCE
	
MAIN:
	; Following only used to trigger an SPC dump by bsnes-plus
	;mov dspaddr, #kon
	;mov dspdata, #1

	CLRP ; set dp to 0
  mov control, #0 ; disable timers and ROM region

  ; We are not dealing with echo yet. so just set it so ff00 at the 
  mov dspaddr, #esa
  mov dspdata, #$ff
  MOV dspaddr, #edl
  MOV dspdata, #0

  mov dspaddr, #dir
  mov dspdata, dspdir_i
  
  ; TODO - Could alter master volume by song setting here
  MOV dspaddr, #mvol_l 
  MOV dspdata, #$40   ; Rather than put on FULL BLAST, try this value
  MOV dspaddr, #mvol_r; used globally in SMRPG
  MOV dspdata, #$40

  ; Todo - enable echo buffer writes when bringing up echo support
  MOV dspaddr, #flg   ; TURN OFF MUTE
  MOV dspdata, #%00100000 ; disable echo buffer writes
	
	;===
	; Load the timer values from their stored settings.

; Enter MAIN LOOP
MainLoop:
	bbc flags.bPlaySong, Poll
  call !ContinueSong

Poll:
  mov a,spcport1  		; Fetch io1
  cmp a,PrevCmd     	; same as PrevCmd?
  beq MainLoop

	nop	; need to waste a cycle to ensure the port data has been loaded. or
	; load spc parameter port
  mov PrevCmd,spcport1     	; Store as PrevCmd
    
  cmp a, #CmdEnd
  bpl PollExit
  ; push the PollExit address to stack and do a table jump
  asl a ; word-sized index into jump table
  mov x, a
  mov a, #>PollExit
  push a
  mov a, #<PollExit
  push a
  jmp [!CmdJumpTable+X]

PollExit:
	mov a,PrevCmd		; pass back cmd to SNES to tell it we're done
	mov spcport1,a	; io1
  bra MainLoop   ; repoll snes

CmdJumpTable:
  .dw EmptyHandler, FetchRamValue, WriteRamByte, PlaySong, StopSong
EmptyHandler:
  ret

; ====== WRITE A BYTE TO THE S-SDSP =========
WDSP:
    MOV $F2,Y
    MOV $F3,A
    RET
; ====== WRITE A BYTE TO THE S-SDSP =========
WDSPX:
    MOV $F2,X
    MOV $F3,A
    RET

.ENDS
