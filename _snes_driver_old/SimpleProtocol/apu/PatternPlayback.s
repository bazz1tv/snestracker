.include "apu/memorymap.i"
.include "apu/enums.i"

.bank 0
.org $200
.section "PatternPlayback" SEMIFREE

PlayPattern:
	jmp !PatternPlayback
	
	

	; TIMER IS STARTED

PatternPlayback:
PATTERNLISTLOOP:
	MOV a, SPCSelectedPatternSequenceCounter
	bne +
	call !LoadPattern

+
	
	
	MOV a, SPCSelectedPatternSequenceCounter
	asl A
	asl A
	mov y,a
	CALL !PatternIteration ; 
		mov a, SPCSelectedPatternSequenceCounter
		cmp a, SPCSelectedPatternLength
		BEQ _DONE 
		ret
_DONE:
	mov a, #0
	mov SPCSelectedPatternSequenceCounter, a
	INC SPCPatternList_CurrentEntry
	ret
	
	; DO THE LOOP FOR VOICES 0-7
PatternIteration:
	
	mov x,SPCCounterAddr
	CLRC
	MOV A, SPCCurrentCountValue
	
	ADC A,(X)	; READ COUNTER
	mov SPCCurrentCountValue,a
	CMP A,SPCTimerCountValue
	BCC _RET
	bra +
_RET:	RET
	
	
+	mov SPCCurrentCountValue, #0
	MOV A,#1
	MOV SPCbPlayANote, A
	MOV A,#0
	MOV SPCbKeyOffNOTE, A
	
	; PITCH CODE
	MOV A, [SPCSelectedPatternPtr]+Y
	INC Y
	MOV SPCLowPitchTemp, A
	MOV A, [SPCSelectedPatternPtr]+Y
	INC Y
	MOV SPCHighPitchTemp, A
	
	; COMPARE FOR 0101
	CMP A, #01
	BNE CMP2
	MOV A, SPCLowPitchTemp
	CMP A, #01
	BNE CMP2
	MOV SPCbKeyOffNOTE, #1
	BRA P2
CMP2:
	CMP A, #00
	BNE SETPITCH
	MOV A, SPCLowPitchTemp
	CMP A,#00
	BNE SETPITCH
	MOV SPCbPlayANote, #0
	BRA P2
SETPITCH:
	MOV A, SPCLowPitchTemp
	MOV X, #V0PL
	CALL !WDSPX
	INC X
	MOV A, SPCHighPitchTemp
	CALL !WDSPX
P2:
	; SAMPLE #
	MOV A, [SPCSelectedPatternPtr]+Y
	INC Y
	MOV X,#$04
	CALL !WDSPX
	
	MOV A, [SPCSelectedPatternPtr]+Y
	INC Y
	; VOLUME
	MOV X,#0
	CALL !WDSPX
	MOV X,#1
	CALL !WDSPX
	
	MOV A,SPCbKeyOffNOTE
	BEQ CHECKFORPLAYANOTE
	MOV A, #1	; PLAY BOTH
	CALL !KEYOFF
	BRA _FINISHUP
CHECKFORPLAYANOTE:	
	MOV A, SPCbPlayANote
	BEQ _FINISHUP
	MOV A, #1
	CALL !KEYON
_FINISHUP:
	INC SPCSelectedPatternSequenceCounter
	RET

TIMERWAIT:
	mov a, SPCTimerSelect
	clrc
	adc a,#COUNTERBASE
	mov x,a
	CLRC
	MOV A, #0
WAIT_FOR_TICK:
	
	ADC A,(X)	; READ COUNTER
	CMP A,SPCTimerCountValue
						; 1 SECOND = 32MS MAX TIMER VALUE * 32 = 1024 MS = 1.024 S
	BCC WAIT_FOR_TICK
	RET
	
STARTOVER:
	MOV SPCPatternList_CurrentEntry, #0
LoadPattern:
	; LOAD ENTRY(X) OF SPCPatternList
	MOV X, SPCPatternList_CurrentEntry
	CMP X, SPCPatternListLength
	BEQ STARTOVER	; NOT CODED YET
	MOV A, SPCPatternList+X
	; HOW TO CONVERT PATTERN NUMBER TO PATTERN TABLE LOCATION
	; PATTERNS START AT PATTERNS AND ARE 3 BYTES EACH
	MOV X,A
	MOV A,#0
GETINDEX:
	CMP X,#0
	BEQ HAVEINDEXINA 
	CLRC
	ADC A,#3
	DEC X
	BRA GETINDEX

HAVEINDEXINA:
	CLRC 
	ADC A,#SPCPatterns
	MOV X,A


	MOV A,(X)+	; PATTERN LENGTH
	MOV !SPCSelectedPatternLength, A
	MOV A,(X)+
	MOV SPCSelectedPatternPtr, A
	MOV A,(X)+
	MOV SPCSelectedPatternPtr+1, A
	ret
	
.ends