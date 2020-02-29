;----------------------------------------------------------------------------
; InitSNES -- my "standard" initialization of SNES memory and registers
;----------------------------------------------------------------------------
.MACRO InitSNES
  sei                     ;disable interrupts
  clc                     ;switch to native mode
  xce

  REP #$38		; mem/A = 16 bit, X/Y = 16 bit
			;decimal mode off

  LDX #$1FFF	;Setup the stack
  TXS			;Transfer Index X to Stack Pointer Register

  ;do the rest of the initialization in a routine
  JSL $008000

  SEP #$20		; mem/A = 8 bit
.ENDM