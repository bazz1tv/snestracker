.include "apu/SPCCommands.i"

;================================
.EQU Control $f1
.EQU DSPAddr $f2
.EQU DSPData $f3

.enum $f4
	spcport0			db
	spcport1			db
	spcport2			db
	spcport3			db
.ende
	
.enum $fa
	Timer0				db
	Timer1				db
	Timer2				db
	Counter0			db
	Counter1			db
	Counter2			db
.ende
;===================================
;DSP Addresses
;===================================
.enum 0
	voice0vol_L	db
	voice0vol_R	db
	voice0pitch_lo	db
	voice0pitch_hi	db
	voice0srcn	db
	voice0adsr1		db
	voice0adsr2		db
	voice0gain		db
	voice0envx		db
	voice0outx		db
.ende
.enum $10
	voice1vol_L	db
	voice1vol_R	db
	voice1pitch_lo	db
	voice1pitch_hi	db
	voice1srcn	db
	voice1adsr1		db
	voice1adsr2		db
	voice1gain		db
	voice1envx		db
	voice1outx		db
.ende
.enum $20
	voice2vol_L	db
	voice2vol_R	db
	voice2pitch_lo	db
	voice2pitch_hi	db
	voice2srcn	db
	voice2adsr1		db
	voice2adsr2		db
	voice2gain		db
	voice2envx		db
	voice2outx		db
.ende
.enum $30
	voice3vol_L	db
	voice3vol_R	db
	voice3pitch_lo	db
	voice3pitch_hi	db
	voice3srcn	db
	voice3adsr1		db
	voice3adsr2		db
	voice3gain		db
	voice3envx		db
	voice3outx		db
.ende
.enum $40
	voice4vol_L	db
	voice4vol_R	db
	voice4pitch_lo	db
	voice4pitch_hi	db
	voice4srcn	db
	voice4adsr1		db
	voice4adsr2		db
	voice4gain		db
	voice4envx		db
	voice4outx		db
.ende
.enum $50
	voice5vol_L	db
	voice5vol_R	db
	voice5pitch_lo	db
	voice5pitch_hi	db
	voice5srcn	db
	voice5adsr1		db
	voice5adsr2		db
	voice5gain		db
	voice5envx		db
	voice5outx		db
.ende
.enum $60
	voice6vol_L	db
	voice6vol_R	db
	voice6pitch_lo	db
	voice6pitch_hi	db
	voice6srcn	db
	voice6adsr1		db
	voice6adsr2		db
	voice6gain		db
	voice6envx		db
	voice6outx		db
.ende
.enum $70
	voice7vol_L	db
	voice7vol_R	db
	voice7pitch_lo	db
	voice7pitch_hi	db
	voice7srcn	db
	voice7adsr1		db
	voice7adsr2		db
	voice7gain		db
	voice7envx		db
	voice7outx		db
.ende

.equ mvol_L $0c
.equ mvol_R $1c
.equ evol_L	$2c
.equ evol_R	$3c
.equ kon	$4c
.equ kof	$5c
.equ flg	$6c
.equ endx	$7c
	
.equ efb		$0d
	; $1d unused
.equ pmon	$2d
.equ non	$3d
.equ eon	$4d
.equ dir	$5d
.equ esa	$6d
.equ edl	$7d
	
.equ c0		$0f
.equ c1		$1f
.equ c2		$2f
.equ c3		$3f
.equ c4		$4f
.equ c5		$5f
.equ c6		$6f
.equ c7		$7f		
;==========

.EQU MAX_PATTERNS 128


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

.ENUM $00;DESC 

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
	SPCPatternList			Dsb MAX_PATTERNS
	
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

