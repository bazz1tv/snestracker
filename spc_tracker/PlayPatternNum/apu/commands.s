.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/enums.i"


.BANK 0 SLOT 2
.ORGA $200
.SECTION "Commands" SEMIFREE

InitTimer:
	; SETUP TIMER
	
	; to get CountAddr
	mov a, SPCTimerSelect
	clrc
	adc a,#COUNTERBASE
	mov SPCCounterAddr,a
	
	
	call !UpdateTimerReg
	
	mov a, SPCTimerSelect
	
	cmp a, #2
	bne +
	inc a
	
+	inc a
	MOV CONTROL, a	; 
	ret

SelectTimer:
	call !WriteRamByte
	call !InitTimer
	ret
	
WriteTimerValue:
	call !WriteRamByte
	call !UpdateTimerReg
	ret
	
UpdateTimerReg:
	MOV a, SPCTimerSelect
	clrc
	adc a, #$fa
	mov SPCTimerAddr, a
	mov x,a
	MOV A,SPCTimerValue
	MOV (X),A
	ret

PlaySong:
	
	
	mov SPCbPlaySong, #1
	ret
	
StopSong:
	mov SPCbPlaySong, #0
	ret
	
real_time_messages:
.db $f8, $f9, $fa, $fb, $fc, $fd, $fe, $ff, $00
MidiByteRoutine:
	mov a,spcport2 ; MIDI Byte
	;----------------------------------------------------
	; MIDI PROCESSING
	;----------------------------------------------------
	; 
	; Check Message vs. Data (MSB set = message)
	mov MidiByte, A
	;mov a,#SPCMidiByteCmd		; pass back cmd to SNES to tell it we're done
	;mov spcport1,a
	;mov A, MidiByte
	and a, #$80
	beq _data
	; Message Section
	;------------------
	; Is it real time message???
	mov X, #0
-	mov A, !real_time_messages+X
	beq _it_is_NOT_real_time_message
	inc X
	cmp A, MidiByte
	bne -
	;IT IS A REAL TIME MESSAGE
	; Not Implemented YET
	jmp !_midi_done
_it_is_NOT_real_time_message:
	;PrintString "Storing in RunningStatusBuffer\n"
	;lda MidiByte.b
	mov a, MidiByte
	mov RunningStatusBuffer, A;sta RunningStatusBuffer.b
	mov ThirdByteFlag, #0;stz ThirdByteFlag.b
	; ------Is this a tune request ?? (NOT IMPLEMENTED NOW)
	;
	;
	bra _processing_loop_end
	;------------------
	; DATA SECTION
	;------------------
_data:
	; Third Byte Flag = 1 ???
	mov A, ThirdByteFlag;lda ThirdByteFlag.b
	beq _ThirdByteFlag_is_zero
	; ThirdByteFlag is 1
	; (code here)
	;
	;
	mov ThirdByteFlag, #0	; stz ThirdByteFlag.b
	mov A, MidiByte 	;lda MidiByte.b
	mov MidiData1, A	;sta MidiData1.b
	;PrintString "Processing a real byte!!!"
	call !MidiCommandProcessor
	bra _processing_loop_end

_ThirdByteFlag_is_zero:
	; Running Status buffer????
	mov A, RunningStatusBuffer	;lda RunningStatusBuffer.b
	beq _processing_loop_end	; IGNORE DATA BYTE if RunningStatusBuffer == 0

	; RunningStatusBuffer != 0 ... 
	cmp A, #$C0
	bcc _commands_less_than_c0

_commands_less_than_c0:
	;PrintString "Processing commands less than C0"
	inc ThirdByteFlag
	;mov A, RunningStatusBuffer.b
	mov MidiStatus, A
	mov A, MidiByte
	mov MidiData0, A
	; Purposely runs into the end of loop below

_midi_done:
_processing_loop_end:


_midi_exit:
	ret
	
MidiCommandProcessor:
	;php
	;rep #$10
	;sep #$20
	
	mov A, MidiStatus	;lda MidiStatus.b
	cmp A, #$90
	bne +
	; NOTEON PROCESSING
	call !MidiNoteCmdProcessing
	bra _end_processing
+
	;else... uncoded
	
_end_processing:
	mov MidiStatus,#0
	mov MidiData0, #0
	mov MidiData1, #0
	ret
	
MidiNoteTable:
	;SNES Pitch Intervals
	;Interval	Pitch Value
	;Root	
		.dw $1000
	;m2	
		.dw $10f0
	;M2	
		.dw $11f0
	;m3	
		.dw $1300
	;M3	
		.dw $1430
	;P4	
		.dw $1560
	;Aug4	
		.dw $16a0
	;P5	
		.dw $1800
	;Aug5	
		.dw $1960
	;M6	
		.dw $1af0
	;m7	
		.dw $1c80
	;M7	
		.dw $1e30
	
MidiNoteCmdProcessing:
	
	; this has to get fetched into a Note Pitch Value Lookup Table
	;
	;
	; for dummy test, let's just use default #$1000
	
	; move the value into Voice0 pitch registers
	
	
	
	;mov a,MidiData1	; velocity, this goes directly into Voice[0] Volume
	mov a, spcport3
	cmp a, #$00
	bne +
	
	;ONLY KeyOff if it's the "KeyOff Note"
	mov A, KeyOffNote
	cmp A, MidiData0
	bne ++
	mov a, #1
	call !KEYOFF
	
	bra ++
+	
	mov $f2, #0
	mov $f3, a
	mov $f2, #1
	mov $f3, a
	;mov a, MidiData0
	mov a,spcport2 ; Note Value
	mov KeyOffNote, A
	setc
	sbc a,#$3c
	asl a
	mov X,A
	; Perform LUT 
	mov a, !MidiNoteTable+X
	mov $f2,#2
	mov $f3,a
	mov a, !MidiNoteTable+1+X
	mov $f2, #3
	mov $f3,a
	;
	
	mov a, #0
	call !KEYOFF
	mov a, #1
	call !KEYON
++
	ret
FetchRamValue:   
    ; fetch RAM value
    mov a,spcport2     	; fetch lo byte if addr from io2
    mov SPCtemp,a     		; store as lo byte of word pointer
    mov a,spcport3     	; fetch high byte from io3
    mov SPCtemp+1,a     	; store as hi byte of word pointer
    mov x,#$00      	; init index to nada
    mov a,[SPCtemp+x]   	; fetch RAM value
    mov spcport2,a     	; pass back to snes via io3
	mov a, SPCtemp
	clrc
	adc a,#1
	bcs _adjustcarry
-	mov SPCtemp, a
	mov a,[SPCtemp+x]
	mov spcport3,a
	ret
_adjustcarry:
	inc SPCtemp+1
	bra -
	
	
	
	
	
; Addr in Port2.3
; Val in Port 0	
WriteRamByte:   
    ; fetch RAM value
    mov a,spcport2     	; fetch lo byte if addr from io2
    mov SPCtemp,a     		; store as lo byte of word pointer
    mov a,spcport3     	; fetch high byte from io3
    mov SPCtemp+1,a     	; store as hi byte of word pointer
	mov a,spcport0
    mov x,#$00      	; init index to nada
    mov [SPCtemp+x],a   ; fetch RAM value
	ret


	;.export derp1,derp2

.ends