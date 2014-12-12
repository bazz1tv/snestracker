; Commands
.enum 0
	SPCempty				db	; Literally Do Nothing Cmd
	
	SPCReadRamCmd			db
	SPCWriteRamByteCmd		db
	
	SPCPlaySongCmd			db
	SPCStopSongCmd			db
	
	SPCSelectTimerCmd		db
	SPCWriteTimerValueCmd	db
.ende