; Map one of the unused auxiliary IO ports to act as a row reporter
; through the use of libgme_M. First, write the data, then activize it
; with the cmd (love making words up recently).
.equ reportTrackerCmd $f8
.equ reportTrackerArg $f9

.enum 0
	reportTrackerCmd_SetRow     db
	reportTrackerCmd_SetPattern db
.ende

.PRINT "#define REPORT_TRACKER_SETROW 0x", HEX reportTrackerCmd_SetRow, "\n"
.PRINT "#define REPORT_TRACKER_SETPATTERN 0x", HEX reportTrackerCmd_SetPattern, "\n"

; We can make struct definitions that don't need to be defined in RAM,
; used purely by their definition as a way to informatically load offsets
; from RAM pointers

.struct PatternSequencer
num_entries   db ;length in bytes (same as number of pattern entries)
sequences     db
/* sequences is described to assembler as 1 byte in size but that is just to
	 make it addressable. In reality, it's the size specified by num_entries.
	 Each entry is just a byte specifying the pattern number */
.ends
