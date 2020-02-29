.include "apu/SPCCommands.i"


.EQU 	CONTROL 		$F1		; TIMER CONTROL, SNES REG CLEAR

.enum $f4
	spcport0			db
	spcport1			db
	spcport2			db
	spcport3			db
.ende

.EQU	TIMERBASE		$FA
.EQU	TIMER0			$FA
.EQU	TIMER1			$FB
.EQU	TIMER2			$FC

.EQU	COUNTERBASE	$FD
.EQU	COUNTER0	$FD
.EQU	COUNTER1	$FE
.EQU	COUNTER2	$FF

.EQU	KON			$4C
.EQU	DSPADDR		$F2
.EQU	DSPVAL		$F3

.EQU	V0PL		$02
.EQU	V0PH		$03

.EQU	V1PL		$12
.EQU	V1PH		$13

; END EQUATES.


; Experimenting. Not UTILIZED YET
.STRUCT PatternEntry
	len		db
	ptr		dw
.ENDST

.ENUM $00 ;DESC 

	SPCSelectedPatternPtr			dw
	
	SPCPattern0Len			db
	SPCPattern0Ptr			dw ; $02
	
	
	SPCPattern1Len			db
	SPCPattern1Ptr			dw ; $02
	
	SPCSelectedPatternLength				db
	SPCSelectedPatternSequenceCounter			db
	SPCPatternList_CurrentEntry		db
	
	SPCTimerSelect			db
	SPCTimerValue			db
	SPCTimerCountValue		dw ;$82 ; -$03
	SPCbPlayANote			db
	SPCbKeyOffNOTE			db
	SPCbFirstPitchWasNot0	db
	SPCLowPitchTemp		db
	SPCHighPitchTemp		db
	SPCtemp				dw
	
	SPCVoiceEnable			db ;$8A
	
	;N LIST ABSOLUTE RAM
	
	SPCPatternListLength	Db
	SPCPatternList			Dsb 8
	
	SPCPrevCmd				db
	SPCSnesBuffer0			db
	SPCSnesBuffer1			db
	
	SPCbPlaySong			db
	SPCTimerAddr			db
	SPCCounterAddr			db
	SPCCurrentCountValue	db
.ende

.equ SPCPatterns			SPCPattern0Len
.equ SPCPattern0PtrLO		SPCPattern0Ptr
.equ SPCPattern0PtrHI		SPCPattern0Ptr+1
.equ SPCPattern1PtrLO		SPCPattern1Ptr
.equ SPCPattern1PtrHI		SPCPattern1Ptr+1

