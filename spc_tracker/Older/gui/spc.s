control 	.equ 	$f1		; Timer Control, SNES reg clear

TIMERBASE	.equ	$fa
timer0		.equ	$fa
timer1		.equ	$fb
timer2		.equ	$fc

COUNTERBASE	.equ	$fd
counter0	.equ	$fd
counter1	.equ	$fe
counter2	.equ	$ff

kon			.equ	$4c
dspaddr		.equ	$f2
dspval		.equ	$f3

v0pl		.equ	$02
v0ph		.equ	$03

v1pl		.equ	$12
v1ph		.equ	$13

PATTERNPTR			.EQU	$00
PATTERNS			.EQU	$02
PATTERN0LEN			.EQU	PATTERNS
PATTERN0PTR			.EQU 	PATTERNS+1 ; $02
PATTERN0PTRLO		.EQU	PATTERNS+2
PATTERN0PTRHI		.EQU	PATTERNS+2

PATTERN1LEN			.EQU	PATTERNS+3
PATTERN1PTR			.EQU 	PATTERNS+4 ; $02
PATTERN1PTRLO		.EQU	PATTERNS+4
PATTERN1PTRHI		.EQU	PATTERNS+5

INVERSEPATTERNENTRYCOUNTER	.EQU	$70
PATTERNLISTNUMBERSCURRENTENTRY		.EQU	$71

TIMERSELECT			.EQU	$80
TIMERVALUE			.EQU	$81
TIMERCOUNTVALUE		.EQU	$82 ; -$03
bplayanote			.EQU 	$85
bkeyoffnote			.equ	$86
firstpitchwasnot0	.equ	$87
lowpitchtemp		.equ	$88
highpitchtemp		.equ	$89

voiceenable			.equ	$8a

; PATTERN LIST ABSOLUTE RAM
PATTERNLISTNUMBERS			.EQU	$90
PATTERNLISTNUMBERSLENGTH	.EQU	$8F




.org $200


	
	
Main:
	clrp
    
	mov $f2, #$7D
	mov $f3, #0

    mov $F2, #$00       ; left volume = 127
    mov $F3, #127
    mov $F2, #$01       ; right volume = 127 
    mov $F3, #127
    
    
    ; DIR
    mov $F2, #$5D   ; starting address = $9000
    mov $F3, #$90
    
	; DIR
	; for piano
    mov $F2, #$04       ; source number = 0 
    mov $F3, #$00
    mov a, #$00
    mov !$9000, a
    mov a, #$10
    mov !$9001, a
    mov a, #$00 ;#$91
    mov !$9002, a
    mov a, #$10    ; #$16
    mov !$9003, a
	
	; for bass
    mov $F2, #$14       ; source number = 1
    mov $F3, #$01
    mov a, #$00
    mov !$9004, a
    mov a, #$40
    mov !$9005, a
    mov a, #$00 ;#$91
    mov !$9006, a
    mov a, #$40    ; #$16
    mov !$9007, a
    
    ; Voice 0 Piano
    mov $F2, #$05 
    mov $F3, #$00 
    mov $F2, #$07 
    mov $F3, #$1F ;#127   ; set GAIN, direct mode (bit7=0), full volume
    ; Voice 1 Bass
    mov $F2, #$15 
    mov $F3, #$00
    mov $F2, #$17 
    mov $F3, #$1F ;#127   ; set GAIN, direct mode (bit7=0), full volume
	
    mov $F2, #$0C 
    mov $F3, #127   ; L/R vol 100% both voices 
    mov $F2, #$1C
    mov $F3, #127
    
    mov $F2, #$6C   ; turn off mute
    mov $F3, #00100000b
	
	; this wasn't used in the pattern Demo
	; INITIALIZE THE PATTERNLISTNUMBERS
	MOV PATTERNLISTNUMBERSLENGTH, #4
	MOV X, #0
	MOV A, #0
	MOV PATTERNLISTNUMBERS+X, A
	INC X
	MOV A, #1
	MOV PATTERNLISTNUMBERS+X, A
	INC X
	MOV A, #1
	MOV PATTERNLISTNUMBERS+X, A
	INC X
	MOV A, #1
	MOV PATTERNLISTNUMBERS+X, A
	; LENGTH 3, PATTERN 0, 1 ,1
	;MOV PATTERNLISTNUMBERSCURRENTENTRY, #0
	
	
	
	
	; INIT PATTERN 0 VOICE 0
	; LETS PLAY SOMETHING AT 0 AND NOTHING FOR ANYTHING ELSE
	; PATTERN FORMAT: NOTE, INSTRUMENT, VOLUME	; 4 BYTES
	; LOOK-UP TABLE FOR KEYBOARD KEYS 0-88
	
	; INIT PATTERN 0 TABLE ENTRY
	MOV A, #8
	mov PATTERN0LEN, A
	MOV A, #00
	MOV PATTERN0PTRLO, A
	mov a, #$82
	MOV PATTERN0PTRHI, A
	
	; INIT PATTERN 1 TABLE ENTRY
	MOV A, #8
	mov PATTERN1LEN, A
	MOV A, #00
	MOV PATTERN1PTRLO, A
	mov a, #$83
	MOV PATTERN1PTRHI, A
	
	CALL INITPATTERNS
	
	; ok the pattern is ready
	; LETS ITERATE THROUGH A PATTERN
	; 
	
	
	;===
	; Store some fake values for the timers
	mov a, #TIMER0
	MOV TIMERSELECT, a
	mov a, #$90
	MOV TIMERVALUE, a
	mov a, #5
	MOV TIMERCOUNTVALUE, a
	; These would normally be selected by the user in the GUI
	
	; Setup Timer
	mov X, TIMERSELECT
	MOV A,TIMERVALUE
	MOV (X),A
	; THAT WILL BE THE STANDARD CONVERSION CODE FOR TIMER SETUPS
	
	; START TIMER
	mov control, #$01	; this will destroy all other timers


	; timer is started
STARTOVER:
	MOV PATTERNLISTNUMBERSCURRENTENTRY, #0
PATTERNLISTLOOP:
	; LOAD ENTRY(X) OF PATTERNLISTNUMBERS
	MOV X, PATTERNLISTNUMBERSCURRENTENTRY
	CMP X, PATTERNLISTNUMBERSLENGTH
	BEQ STARTOVER	; NOT CODED YET
	MOV A, PATTERNLISTNUMBERS+X
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
	ADC A,#PATTERNS
	MOV X,A
	
	
	mov a,(X)+	; PATTERN LENGTH
	MOV !INVERSEPATTERNENTRYCOUNTER, a
	MOV A,(X)+
	MOV PATTERNPTR, A
	MOV A,(X)+
	MOV PATTERNPTR+1, A
	
	MOV Y, #0
	
	CALL PATTERNLOOP
	INC PATTERNLISTNUMBERSCURRENTENTRY
	BRA PATTERNLISTLOOP
	
	; Do the Loop for Voices 0-7
PATTERNLOOP:
	CALL TIMERWAIT
	mov a,#1
	MOV bplayanote, a
	mov a,#0
	mov bkeyoffnote, a
	
	; Pitch Code
	MOV A, [PATTERNPTR]+Y
	inc y
	mov lowpitchtemp, a
	MOV A, [PATTERNPTR]+Y
	INC Y
	mov highpitchtemp, a
	
	; compare for 0101
	cmp a, #01
	bne cmp2
	mov a, lowpitchtemp
	cmp a, #01
	bne cmp2
	mov bkeyoffnote, #1
	bra p2
cmp2:
	cmp a, #00
	bne setpitch
	mov a, lowpitchtemp
	cmp a,#00
	bne setpitch
	mov bplayanote, #0
	bra p2
setpitch:
	mov a, lowpitchtemp
	mov x, #v0pl
	call wdspx
	inc X
	mov a, highpitchtemp
	call wdspx
p2:
	; Sample #
	MOV A, [PATTERNPTR]+Y
	INC Y
	MOV X,#$04
	CALL WDSPx
	
	MOV A, [PATTERNPTR]+Y
	INC Y
	; Volume
	MOV x,#0
	CALL WDSPx
	MOV X,#1
	CALL WDSPx
	
	MOV A,bkeyoffnote
	BEQ checkforplayanote
	mov a, #1b	; play both
	call keyoff
	bra _finishup
checkforplayanote:	
	mov a, bplayanote
	beq _finishup
	mov a, #1
	call keyon
_finishup:
	DEC INVERSEPATTERNENTRYCOUNTER
	Beq _done 
	jmp PATTERNLOOP
_done:
	RET

TIMERWAIT:
	clrc
	mov a, #0
wait_for_tick:

	adc a,COUNTER0	; read counter
	cmp a,timercountvalue
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick
	RET
    
    
; ====== Write a byte to the S-SDSP =========
wdsp:
    mov !$00F2,y
    mov !$00F3,a
    ret
; ====== Write a byte to the S-SDSP =========
wdspx:
    mov !$00F2,X
    mov !$00F3,a
    ret
	
keyon:
	; Key ON
	mov $F2, #$4C
	mov $F3, a
	ret
	
keyoff:
	mov $f2, #$5c
	mov $f3, a
	ret
    

INITPATTERNS:
	; INIT PATTERN 0 PATTERN DATA
	
	MOV Y, #0
	
	; 1
	MOV A, #$10
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV A, #$10
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV A,#01
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	inc y
	
	; 2
	MOV A, #00
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV [PATTERN0PTR]+Y,A
	INC Y
	mov a, #01
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	INC Y

	;3
	MOV A, #$10
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A, #$18
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#01
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	INC Y

	;4
	MOV A, #00
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV [PATTERN0PTR]+Y,A
	INC Y
	mov a,#1
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	INC Y

	;5
	MOV A, #$10
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A, #$20
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#01
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	INC Y

	;6
	MOV A, #00
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV [PATTERN0PTR]+Y,A
	INC Y
	mov a, #1
	MOV [PATTERN0PTR]+Y,A
	INC Y
	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	INC Y
	
	;7
	MOV A, #$10
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A, #$18
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#01
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	INC Y
	
	;8
	MOV A, #$00
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A, #$01
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#01
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV A,#127
	MOV [PATTERN0PTR]+Y,A
	INC Y

	MOV Y,#0

	; INIT PATTERN 1
	
	; 1
	MOV Y, #0
	MOV A, #$10
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV A, #$10
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV A,#01
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV A,#127
	MOV [PATTERN1PTR]+Y,A
	inc y
	
	MOV A, #00
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a,#1
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #127
	MOV [PATTERN1PTR]+Y,A
	INC Y




	MOV A, #00
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a,#1
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #127
	MOV [PATTERN1PTR]+Y,A
	INC Y



	MOV A, #00
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #1
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #127
	MOV [PATTERN1PTR]+Y,A
	INC Y

	MOV A, #$10
	MOV [PATTERN1PTR]+Y,A
	INC Y

	MOV A, #$18
	MOV [PATTERN1PTR]+Y,A
	INC Y

	MOV A,#01
	MOV [PATTERN1PTR]+Y,A
	INC Y

	MOV A,#127
	MOV [PATTERN1PTR]+Y,A
	INC Y

	mov a,#0
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #1
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #127
	MOV [PATTERN1PTR]+Y,A
	INC Y




	MOV A, #00
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a,#1
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #127
	MOV [PATTERN1PTR]+Y,A
	INC Y



	MOV A, #00
	MOV [PATTERN1PTR]+Y,A
	INC Y
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #1
	MOV [PATTERN1PTR]+Y,A
	INC Y
	mov a, #127
	MOV [PATTERN1PTR]+Y,A
	INC Y
	RET

.END
EOF