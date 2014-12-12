.include "cpu/enums.i"
.include "apu/enums.i"
.enum enumend EXPORT

SNESPatternListLength	Db
SNESPatternList_CurrentEntry db
SNESPatternList			Dsb MAX_PATTERNS

.ende