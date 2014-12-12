.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/enums.i"


.BANK 0 SLOT 2
.ORGA $200
.SECTION "Commands" SEMIFREE

;.define SAMPLE1START SPCMAIN+SPC_CODE_BLOCKSIZE
;.define SAMPLE1SIZE piano_end-piano_start
;.export SAMPLE1START, SAMPLE1SIZE
;.define SAMPLE2START SAMPLE1START+SAMPLE1SIZE
;.define SAMPLE2SIZE bass_end-bass_start
;.export SAMPLE2START,SAMPLE2SIZE

;.dw SAMPLE1START, SAMPLE1SIZE
SAMPLE1START:
	.INCBIN "apu/samples/piano.brr"
sample1_end:
;	.dw SAMPLE2START, SAMPLE2SIZE
;SAMPLE2START:
;	.INCBIN "apu/samples/piano.brr"
;sample2_end:
	
.ends