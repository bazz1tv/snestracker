.include "cpu/memorymap.i"

.bank 0
.orga $8000
.section "vblank" SEMIFREE
VBlank:
	rep #$30		;A/Mem=16bits, X/Y=16bits
	phb
	pha
	phx
	phy
	phd
	lda $4210		;clear NMI Flag
	JML [VBlankPtr]
	
ReturnVBlank:
	rep #$30
	PLD 
	PLY 
	PLX 
	PLA 
	PLB 
    RTI
	
.ends