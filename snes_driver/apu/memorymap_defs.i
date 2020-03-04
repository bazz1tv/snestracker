/* By putting these defs in a separate include file from memorymap.i, we
can let cpu code know of the spc blob start address automatically. That
works by not including the memory map directives.
*/
.enum 0
  SPC_DP0_SLOT db
  SPC_DP1_SLOT db
  SPC_RAM_SLOT db
  SPC_CODE_SLOT db
.ende

.equ SPC_RAM_START $200
.equ SPC_RAM_SIZE  $200

.equ SPC_CODE_START (SPC_RAM_START + SPC_RAM_SIZE)
.equ SPC_CODE_SIZE ($10000 - SPC_CODE_START)

.PRINT "#define SPCDRIVER_CODESTART 0x", HEX SPC_CODE_START, "\n"
