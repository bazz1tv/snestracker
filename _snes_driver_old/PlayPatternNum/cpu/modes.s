.include "cpu/memorymap.i"
; MODES
.enum $00 EXPORT
	TimerOptionsMode_Nav		db
    TimerOptionsMode_Row0		db
	TimerOptionsMode_Row1 		db
	TimerOptionsMode_Row2 		db
	
	PatternListMode_Nav			db
	PatternListMode_Edit		db
.ende