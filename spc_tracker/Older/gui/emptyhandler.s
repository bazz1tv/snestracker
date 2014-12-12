.include "header.i"

.BANK 0 SLOT 0                  ; Defines the ROM bank and the slot it is inserted in memory.
.ORG 0                          ; .ORG 0 is really $8000, because the slot starts at $8000
.SECTION "EmptyVectors" SEMIFREE

EmptyHandler:
        rti

.ENDS