/* By putting these defs in a separate include file from memorymap.i, we
can let cpu code know of the spc blob start address automatically. That
works by not including the memory map directives.
*/

; we don't need any additional RAM atm, because zero page is so plentiful
.equ SPC_RAM_START $0200
.equ SPC_RAM_SIZE  $02

.equ SPC_CODE_START (SPC_RAM_START + SPC_RAM_SIZE)
.equ SPC_CODE_SIZE ($10000 - SPC_CODE_START)

.PRINT "#define SPCDRIVER_CODESTART 0x", HEX SPC_CODE_START, "\n"
