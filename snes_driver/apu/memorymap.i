;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; universal defines
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.include "apu/memorymap_defs.i"

.MEMORYMAP
DefaultSlot SPC_CODE_SLOT
slot SPC_DP0_SLOT $00 $100
slot SPC_DP1_SLOT $00 $0c0 ; Take care to leave room for stack!
slot SPC_RAM_SLOT SPC_RAM_START SPC_RAM_SIZE
slot SPC_CODE_SLOT SPC_CODE_START SPC_CODE_SIZE
.endme

.ROMBANKMAP
Bankstotal 1
Banksize SPC_CODE_SIZE
Banks 1
.ENDRO

.EMPTYFILL $FF
