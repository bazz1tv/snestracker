.include "cpu/memorymap.i"

.bank 0
.org 0
.section "derp" FORCE
	; Pattern 0
.db $10, $10, 1, 127
.db $0, $0, 1, 127
.db $10, $18, 1, 127
.db 0,0,1,127	
.db 16, $20, 1, 127
.db 0, 0,1,127
.db $10, $18, 1, 127
.db 0, 01, 1, 127


	; INIT PATTERN 1
.db $10,$10,1,127
.db 0,0,1,127	
.db 0,0,1,127	
.db 0,0,1,127	
.db $10, $18, 1, 127
.db 0,0,1,127	
.db 0,0,1,127	
.db 0,0,1,127
.ends
