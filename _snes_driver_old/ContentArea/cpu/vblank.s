.include "cpu/memorymap.i"

.bank 0
.org 0 
.section "vblank"
VBlank:
	rep #$30		;A/Mem=16bits, X/Y=16bits
	phb
	pha
	phx
	phy
	phd
	
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